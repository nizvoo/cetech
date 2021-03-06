#include <string.h>

#include "celib/id.h"
#include "celib/config.h"
#include "celib/memory/memory.h"
#include <celib/module.h>
#include <celib/ydb.h>
#include <celib/math/math.h>

#include <celib/macros.h>
#include "celib/api.h"

#include "cetech/resource/resource.h"
#include "cetech/ecs/ecs.h"

#include "cetech/transform/transform.h"
#include <cetech/renderer/renderer.h>
#include <cetech/renderer/gfx.h>
#include <cetech/scene/scene.h>
#include <cetech/material/material.h>
#include <cetech/mesh/mesh_renderer.h>

#include <celib/memory/allocator.h>
#include <celib/macros.h>
#include <cetech/debugui/icons_font_awesome.h>
#include <celib/ydb.h>
#include <cetech/editor/editor_ui.h>
#include <celib/log.h>
#include <celib/containers/buffer.h>
#include <cetech/editor/property.h>
#include <cetech/render_graph/render_graph.h>
#include <cetech/default_rg/default_rg.h>
#include <cetech/camera/camera.h>


#define LOG_WHERE "mesh_renderer"

#define _G mesh_render_global

static struct _G {
    ce_alloc_t0 *allocator;
} _G;


typedef struct mesh_render_data {
    uint8_t viewid;
    uint64_t layer_name;
} mesh_render_data;

void foreach_mesh_renderer(ct_world_t0 world,
                           struct ct_entity_t0 *entities,
                           ct_entity_storage_o0 *item,
                           uint32_t n,
                           void *_data) {
    mesh_render_data *data = _data;

    uint64_t *transforms = ct_ecs_a0->get_all(TRANSFORM_COMPONENT, item);
    uint64_t *mesh_renderers = ct_ecs_a0->get_all(MESH_RENDERER_COMPONENT, item);

    for (int i = 0; i < n; ++i) {
        const ce_cdb_obj_o0 *tr = ce_cdb_a0->read(ce_cdb_a0->db(), transforms[i]);

        ct_mesh_component m_c = {};
        ce_cdb_a0->read_to(ce_cdb_a0->db(), mesh_renderers[i], &m_c, sizeof(m_c));

        if (!m_c.scene || !m_c.material) {
            continue;
        }

        float *final_w = ce_cdb_a0->read_blob(tr, PROP_WORLD, NULL, CE_MAT4_IDENTITY);

        uint64_t mesh_id = ce_id_a0->id64(m_c.mesh);
        const ce_cdb_obj_o0 *scene_reader = ce_cdb_a0->read(ce_cdb_a0->db(), m_c.scene);

        uint64_t mesh = mesh_id;
        uint64_t geom_objs = ce_cdb_a0->read_subobject(scene_reader, SCENE_GEOM_OBJS, 0);
        const ce_cdb_obj_o0 *geom_objs_r = ce_cdb_a0->read(ce_cdb_a0->db(), geom_objs);

        uint64_t geom_obj = ce_cdb_a0->read_ref(geom_objs_r, mesh, 0);

        if (!geom_obj) {
            continue;
        }

        ct_scene_geom_obj_t0 go = {};
        ce_cdb_a0->read_to(ce_cdb_a0->db(), geom_obj, &go, sizeof(go));

        bgfx_index_buffer_handle_t ibh = {.idx = (uint16_t) go.ib};
        bgfx_vertex_buffer_handle_t vbh = {.idx = (uint16_t) go.vb};

        ct_gfx_a0->bgfx_set_transform(final_w, 1);
        ct_gfx_a0->bgfx_set_vertex_buffer(0, vbh, 0, (uint32_t) go.vb_size);
        ct_gfx_a0->bgfx_set_index_buffer(ibh, 0, (uint32_t) go.ib_size);

        ct_material_a0->submit(m_c.material, data->layer_name, data->viewid);
    }
}

static void _init_api(struct ce_api_a0 *api) {
}

void mesh_combo_items(uint64_t obj,
                      char **items,
                      uint32_t *items_count) {
    const ce_cdb_obj_o0 *reader = ce_cdb_a0->read(ce_cdb_a0->db(), obj);

    uint64_t scene_id = ce_cdb_a0->read_ref(reader, PROP_SCENE_ID, 0);

    if (!scene_id) {
        return;
    }

    ct_scene_a0->get_all_geometries(scene_id, items, items_count);
}

void node_combo_items(uint64_t obj,
                      char **items,
                      uint32_t *items_count) {
    const ce_cdb_obj_o0 *reader = ce_cdb_a0->read(ce_cdb_a0->db(), obj);

    uint64_t scene_id = ce_cdb_a0->read_ref(reader, PROP_SCENE_ID, 0);

    if (!scene_id) {
        return;
    }


    ct_scene_a0->get_all_nodes(scene_id, items, items_count);
}

static uint64_t cdb_type() {
    return MESH_RENDERER_COMPONENT;
}

static const char *display_name() {
    return ICON_FA_HOUZZ " Mesh renderer";
}

static void property_editor(uint64_t obj,
                            uint64_t context) {

    ct_editor_ui_a0->prop_resource(obj,
                                   PROP_SCENE_ID, "Scene", PROP_SCENE_ID, context, obj);

    ct_editor_ui_a0->prop_str_combo(obj,
                                    PROP_MESH, "Mesh", mesh_combo_items, obj);

    ct_editor_ui_a0->prop_str_combo(obj,
                                    PROP_NODE, "Node", node_combo_items, obj);

    ct_editor_ui_a0->prop_resource(obj,
                                   ce_id_a0->id64("material"), "Material",
                                   ce_id_a0->id64("material"), context, obj + 1);

}

static struct ct_property_editor_i0 property_editor_api = {
        .cdb_type = cdb_type,
        .draw_ui = property_editor,
};

void render(ct_world_t0 world,
            struct ct_rg_builder_t0 *builder) {

    uint8_t viewid = builder->get_layer_viewid(builder, _GBUFFER);


    mesh_render_data render_data = {
            .viewid = viewid,
            .layer_name = _GBUFFER,
    };

    ct_ecs_a0->process(world,
                       ct_ecs_a0->mask(MESH_RENDERER_COMPONENT) |
                       ct_ecs_a0->mask(TRANSFORM_COMPONENT),
                       foreach_mesh_renderer, &render_data);
}

static struct ct_renderer_component_i0 ct_renderer_component_i = {
        .render = render
};

static void *get_interface(uint64_t name_hash) {
    if (EDITOR_COMPONENT == name_hash) {
        static struct ct_editor_component_i0 ct_editor_component_i0 = {
                .display_name = display_name,
        };

        return &ct_editor_component_i0;
    } else if (CT_RENDERER_COMPONENT_I == name_hash) {
        return &ct_renderer_component_i;
    }

    return NULL;
}

static struct ct_component_i0 ct_component_api = {
        .cdb_type = cdb_type,
        .get_interface = get_interface,
};

static ce_cdb_prop_def_t0 mesh_renderer_component_prop[] = {
        {.name = "material",
                .type = CDB_TYPE_REF,
        },

        {.name = "scene",
                .type = CDB_TYPE_REF,
        },

        {.name = "node",
                .type = CDB_TYPE_STR,
        },

        {.name = "mesh",
                .type = CDB_TYPE_STR,
        },
};

static void _init(struct ce_api_a0 *api) {
    _init_api(api);

    _G = (struct _G) {
            .allocator = ce_memory_a0->system,
    };

    api->register_api(COMPONENT_INTERFACE, &ct_component_api, sizeof(ct_component_api));
    api->register_api(PROPERTY_EDITOR_INTERFACE, &property_editor_api, sizeof(property_editor_api));

    ce_cdb_a0->reg_obj_type(MESH_RENDERER_COMPONENT,
                            mesh_renderer_component_prop,
                            CE_ARRAY_LEN(mesh_renderer_component_prop));
}

static void _shutdown() {
}

static void init(struct ce_api_a0 *api) {
    _init(api);
}

static void shutdown() {
    _shutdown();
}

void CE_MODULE_LOAD(mesh_renderer)(struct ce_api_a0 *api,
                                   int reload) {
    CE_UNUSED(reload);
    CE_INIT_API(api, ce_memory_a0);
    CE_INIT_API(api, ce_id_a0);
    CE_INIT_API(api, ct_material_a0);
    CE_INIT_API(api, ce_ydb_a0);
    CE_INIT_API(api, ce_ydb_a0);
    CE_INIT_API(api, ct_scene_a0);
    CE_INIT_API(api, ct_ecs_a0);
    CE_INIT_API(api, ce_cdb_a0);
    CE_INIT_API(api, ct_resource_a0);
    CE_INIT_API(api, ct_renderer_a0);
    init(api);
}

void CE_MODULE_UNLOAD(mesh_renderer)(struct ce_api_a0 *api,
                                     int reload) {

    CE_UNUSED(reload);
    CE_UNUSED(api);

    shutdown();
}
