#include <cetech/application/private/module.h>
#include "../luasys.h"
#include <cetech/input/input.h>

#define API_NAME "Keyboard"

static struct KeyboardApiV0 KeyboardApiV0 = {0};


static int _keyboard_button_index(lua_State *l) {
    const char *name = luasys_to_string(l, 1);

    u32 idx = KeyboardApiV0.button_index(name);
    luasys_push_float(l, idx);

    return 1;
}

static int _keyboard_button_name(lua_State *l) {
    u32 idx = (u32) (luasys_to_f32(l, 1));

    luasys_push_string(l, KeyboardApiV0.button_name(idx));

    return 1;

}

static int _keyboard_button_state(lua_State *l) {
    u32 idx = (u32) (luasys_to_f32(l, 1));

    luasys_push_bool(l, KeyboardApiV0.button_state(0, idx));

    return 1;
}

static int _keyboard_button_pressed(lua_State *l) {
    u32 idx = (u32) (luasys_to_f32(l, 1));

    luasys_push_bool(l, KeyboardApiV0.button_pressed(0, idx));

    return 1;

}

static int _keyboard_button_released(lua_State *l) {
    u32 idx = (u32) (luasys_to_f32(l, 1));

    luasys_push_bool(l, KeyboardApiV0.button_released(0, idx));

    return 1;

}


void _register_lua_keyboard_api(get_api_fce_t get_engine_api) {
    KeyboardApiV0 = *((struct KeyboardApiV0 *) get_engine_api(KEYBOARD_API_ID,
                                                              0));

    luasys_add_module_function(API_NAME, "button_index",
                               _keyboard_button_index);
    luasys_add_module_function(API_NAME, "button_name", _keyboard_button_name);
    luasys_add_module_function(API_NAME, "button_state",
                               _keyboard_button_state);
    luasys_add_module_function(API_NAME, "button_pressed",
                               _keyboard_button_pressed);
    luasys_add_module_function(API_NAME, "button_released",
                               _keyboard_button_released);
}