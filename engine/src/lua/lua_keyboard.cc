#include "lua/lua_enviroment.h"
#include "lua/lua_stack.h"

#include "os/os.h"

namespace cetech {
    static int keyboard_button_index(lua_State* L) {
        LuaStack stack(L);
        
        const char* scancode = stack.to_string(1);
        const uint32_t index = os::keyboard::button_index(scancode);
        stack.push_uint32(index);
        return 1;
    }

    static int keyboard_button_name(lua_State* L) {
        LuaStack stack(L);
        
        uint32_t index = stack.to_int(1);
        stack.push_string(os::keyboard::button_name(index));
        return 1;
    }

    static int keyboard_button_state(lua_State* L) {
        LuaStack stack(L);
        
        uint32_t index = stack.to_int(1);
        stack.push_bool(os::keyboard::button_state(index));
        return 1;
    }

    static int keyboard_button_pressed(lua_State* L) {
        LuaStack stack(L);
        
        uint32_t index = stack.to_int(1);
        stack.push_bool(os::keyboard::button_pressed(index));
        return 1;
    }

    static int keyboard_button_released(lua_State* L) {
        LuaStack stack(L);
        
        uint32_t index = stack.to_int(1);
        stack.push_bool(os::keyboard::button_released(index));
        return 1;
    }

    namespace lua_keyboard {
        static const char* module_name = "Keyboard";

        void load_libs(LuaEnviroment& env) {
            env.set_module_function(module_name, "button_index", keyboard_button_index);
            env.set_module_function(module_name, "button_name", keyboard_button_name);
            env.set_module_function(module_name, "button_state", keyboard_button_state);
            env.set_module_function(module_name, "pressed", keyboard_button_pressed);
            env.set_module_function(module_name, "released", keyboard_button_released);
        }
    }
}