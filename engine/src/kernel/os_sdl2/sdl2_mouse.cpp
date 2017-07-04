//==============================================================================
// Incldues
//==============================================================================

#include <include/SDL2/SDL.h>

#include <cetech/kernel/module.h>
#include <cetech/celib/eventstream.inl>
#include <cetech/kernel/os.h>

#include <cetech/kernel/application.h>
#include <cetech/modules/resource.h>
#include <cetech/kernel/machine.h>
#include <cetech/kernel/api_system.h>

using namespace cetech;

//==============================================================================
// Defines
//==============================================================================

#define is_button_down(now, last) ((now) && !(last))
#define is_button_up(now, last)   (!(now) && (last))


//==============================================================================
// Globals
//==============================================================================

static struct G {
    uint8_t state[MOUSE_BTN_MAX];
    int position[2];
} _G = {0};


CETECH_DECL_API(app_api_v0);
CETECH_DECL_API(os_window_api_v0);

//==============================================================================
// Interface
//==============================================================================

int sdl_mouse_init(struct api_v0 *api) {
    _G = (struct G) {0};

    CETECH_GET_API(api, app_api_v0);
    CETECH_GET_API(api, os_window_api_v0);

    return 1;
}

void sdl_mouse_shutdown() {
    _G = (struct G) {0};
}

void sdl_mouse_process(EventStream &stream) {
    int pos[2] = {0};

    uint32_t state = SDL_GetMouseState(&pos[0], &pos[1]);

    uint8_t curent_state[MOUSE_BTN_MAX] = {0};

    curent_state[MOUSE_BTN_LEFT] = (uint8_t) (state & SDL_BUTTON_LMASK);
    curent_state[MOUSE_BTN_RIGHT] = (uint8_t) (state & SDL_BUTTON_RMASK);
    curent_state[MOUSE_BTN_MIDLE] = (uint8_t) (state & SDL_BUTTON_MMASK);

    if ((pos[0] != _G.position[0]) || (pos[1] != _G.position[1])) {
        auto *main_window = app_api_v0.main_window();

        uint32_t window_size[2] = {0};
        os_window_api_v0.size(main_window, &window_size[0],
                              &window_size[1]);

        _G.position[0] = pos[0];
        _G.position[1] = window_size[1] - pos[1];

        struct mouse_move_event event;
        event.pos.x = pos[0];
        event.pos.y = window_size[1] - pos[1];

        eventstream::push(stream, EVENT_MOUSE_MOVE, event);
    }

    for (uint32_t i = 0; i < MOUSE_BTN_MAX; ++i) {
        struct mouse_event event;
        event.button = i;

        if (is_button_down(curent_state[i], _G.state[i]))
            eventstream::push(stream, EVENT_MOUSE_DOWN, event);

        else if (is_button_up(curent_state[i], _G.state[i]))
            eventstream::push(stream, EVENT_MOUSE_UP, event);

        _G.state[i] = curent_state[i];
    }
}
