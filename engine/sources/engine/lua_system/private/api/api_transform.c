
#include <engine/entcom/types.h>
#include <engine/world_system/transform.h>
#include <celib/math/mat44f.h>
#include "engine/lua_system/lua_system.h"

#define API_NAME "Transform"

static int _transform_get(lua_State *l) {
    world_t w = {.h = luasys_to_handler(l, 1)};
    entity_t ent = {.h = luasys_to_handler(l, 2)};

    luasys_push_int(l, transform_get(w, ent).idx);
    return 1;
}

static int _transform_get_position(lua_State *l) {
    world_t w = {.h = luasys_to_handler(l, 1)};
    transform_t t = {.idx = luasys_to_int(l, 2)};

    luasys_push_vector3(l, transform_get_position(w, t));
    return 1;
}

static int _transform_get_rotation(lua_State *l) {
    world_t w = {.h = luasys_to_handler(l, 1)};
    transform_t t = {.idx = luasys_to_int(l, 2)};

    luasys_push_quat(l, transform_get_rotation(w, t));
    return 1;
}

static int _transform_get_scale(lua_State *l) {
    world_t w = {.h = luasys_to_handler(l, 1)};
    transform_t t = {.idx = luasys_to_int(l, 2)};

    luasys_push_vector3(l, transform_get_scale(w, t));
    return 1;
}

static int _transform_set_position(lua_State *l) {
    world_t w = {.h = luasys_to_handler(l, 1)};
    transform_t t = {.idx = luasys_to_int(l, 2)};
    vec3f_t *pos = luasys_to_vector3(l, 3);

    transform_set_position(w, t, *pos);
    return 0;
}

static int _transform_set_scale(lua_State *l) {
    world_t w = {.h = luasys_to_handler(l, 1)};
    transform_t t = {.idx = luasys_to_int(l, 2)};
    vec3f_t *pos = luasys_to_vector3(l, 3);

    transform_set_scale(w, t, *pos);
    return 0;
}

static int _transform_set_rotation(lua_State *l) {
    world_t w = {.h = luasys_to_handler(l, 1)};
    transform_t t = {.idx = luasys_to_int(l, 2)};
    quatf_t *rot = luasys_to_quat(l, 3);

    transform_set_rotation(w, t, *rot);
    return 0;
}

static int _transform_get_world_matrix(lua_State *l) {
    world_t w = {.h = luasys_to_handler(l, 1)};
    transform_t t = {.idx = luasys_to_int(l, 2)};

    mat44f_t *wm = transform_get_world_matrix(w, t);

    luasys_push_mat44f(l, *wm);
    return 1;
}

void _register_lua_transform_api() {
    luasys_add_module_function(API_NAME, "get", _transform_get);

    luasys_add_module_function(API_NAME, "get_position", _transform_get_position);
    luasys_add_module_function(API_NAME, "get_rotation", _transform_get_rotation);
    luasys_add_module_function(API_NAME, "get_scale", _transform_get_scale);
    luasys_add_module_function(API_NAME, "get_world_matrix", _transform_get_world_matrix);

    luasys_add_module_function(API_NAME, "set_position", _transform_set_position);
    luasys_add_module_function(API_NAME, "set_rotation", _transform_set_rotation);
    luasys_add_module_function(API_NAME, "set_scale", _transform_set_scale);
}