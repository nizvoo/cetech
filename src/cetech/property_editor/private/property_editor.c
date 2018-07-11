#include <corelib/ebus.h>
#include <corelib/macros.h>
#include "corelib/hashlib.h"
#include "corelib/memory.h"
#include "corelib/api_system.h"
#include "corelib/module.h"

#include <cetech/debugui/debugui.h>

#include <cetech/property_editor/property_editor.h>
#include <cetech/playground/playground.h>
#include <cetech/debugui/private/iconfontheaders/icons_font_awesome.h>
#include <cetech/dock/dock.h>

#define WINDOW_NAME "Property editor"

#define _G property_inspector_global
static struct _G {
    bool visible;
} _G;

static struct ct_property_editor_a0 property_inspector_api = {

};

struct ct_property_editor_a0 *ct_property_editor_a0 = &property_inspector_api;

static void on_debugui(struct ct_dock_i0 *dock) {
    struct ct_api_entry it = ct_api_a0->first("ct_property_editor_i0");
    while (it.api) {
        struct ct_property_editor_i0 *i = (it.api);
        i->draw();

        it = ct_api_a0->next(it);
    }
}

static const char *dock_title() {
    return ICON_FA_TABLE " " WINDOW_NAME;
}

static const char *name(struct ct_dock_i0 *dock) {
    return "property_editor";
}

static struct ct_dock_i0 ct_dock_i0 = {
        .id = 0,
        .visible = true,
        .name = name,
        .display_title = dock_title,
        .draw_ui = on_debugui,
};

static void _init(struct ct_api_a0 *api) {
    _G = (struct _G) {
            .visible = true
    };

    api->register_api("ct_property_editor_a0", &property_inspector_api);
    api->register_api("ct_dock_i0", &ct_dock_i0);

}

static void _shutdown() {
    _G = (struct _G) {};
}

CETECH_MODULE_DEF(
        property_inspector,
        { ;
            CETECH_GET_API(api, ct_hashlib_a0);
        },
        {
            CT_UNUSED(reload);
            _init(api);
        },
        {
            CT_UNUSED(reload);
            CT_UNUSED(api);
            _shutdown();
        }
)