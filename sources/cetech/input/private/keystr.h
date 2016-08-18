//==============================================================================
// Includes
//==============================================================================

#include "cetech/machine/types.h"

//TODO: string
static const char *_key_to_str[512] = {
        [KEY_UNKNOWN] = "",
        [KEY_A] = "a",
        [KEY_B] = "b",
        [KEY_C] = "c",
        [KEY_D] = "d",
        [KEY_E] = "e",
        [KEY_F] = "f",
        [KEY_G] = "g",
        [KEY_H] = "h",
        [KEY_I] = "i",
        [KEY_J] = "j",
        [KEY_K] = "k",
        [KEY_L] = "l",
        [KEY_M] = "m",
        [KEY_N] = "n",
        [KEY_O] = "o",
        [KEY_P] = "p",
        [KEY_Q] = "q",
        [KEY_R] = "r",
        [KEY_S] = "s",
        [KEY_T] = "t",
        [KEY_U] = "u",
        [KEY_V] = "v",
        [KEY_W] = "w",
        [KEY_X] = "x",
        [KEY_Y] = "y",
        [KEY_Z] = "z",

        [KEY_1] = "1",
        [KEY_2] = "2",
        [KEY_3] = "3",
        [KEY_4] = "4",
        [KEY_5] = "5",
        [KEY_6] = "6",
        [KEY_7] = "7",
        [KEY_8] = "8",
        [KEY_9] = "9",
        [KEY_0] = "0",

        [KEY_RETURN] = "return",
        [KEY_ESCAPE] = "escape",
        [KEY_BACKSPACE] = "backspace",
        [KEY_TAB] = "tab",
        [KEY_SPACE] = "space",

        [KEY_MINUS] = "-",
        [KEY_EQUALS] = "=",
        [KEY_LEFTBRACKET] = "(",
        [KEY_RIGHTBRACKET] = ")",
        [KEY_BACKSLASH] = "",
        [KEY_NONUSHASH] = "",
        [KEY_SEMICOLON] = "",
        [KEY_APOSTROPHE] = "",
        [KEY_GRAVE] = "",
        [KEY_COMMA] = "",
        [KEY_PERIOD] = "",
        [KEY_SLASH] = "",

        [KEY_CAPSLOCK] = "caps",

        [KEY_F1] = "f1",
        [KEY_F2] = "f2",
        [KEY_F3] = "f3",
        [KEY_F4] = "f4",
        [KEY_F5] = "f5",
        [KEY_F6] = "f6",
        [KEY_F7] = "f7",
        [KEY_F8] = "f8",
        [KEY_F9] = "f9",
        [KEY_F10] = "f10",
        [KEY_F11] = "f11",
        [KEY_F12] = "f12",

        [KEY_PRINTSCREEN] = "printscreen",
        [KEY_SCROLLLOCK] = "scrolllock",
        [KEY_PAUSE] = "pause",
        [KEY_INSERT] = "insert",

        [KEY_HOME] = "home",
        [KEY_PAGEUP] = "pageup",
        [KEY_DELETE] = "delete",
        [KEY_END] = "end",
        [KEY_PAGEDOWN] = "pagedown",
        [KEY_RIGHT] = "right",
        [KEY_LEFT] = "left",
        [KEY_DOWN] = "down",
        [KEY_UP] = "up",

        [KEY_NUMLOCKCLEAR] = "",
        [KEY_KP_DIVIDE] = "",
        [KEY_KP_MULTIPLY] = "",
        [KEY_KP_MINUS] = "",
        [KEY_KP_PLUS] = "",
        [KEY_KP_ENTER] = "",
        [KEY_KP_1] = "",
        [KEY_KP_2] = "",
        [KEY_KP_3] = "",
        [KEY_KP_4] = "",
        [KEY_KP_5] = "",
        [KEY_KP_6] = "",
        [KEY_KP_7] = "",
        [KEY_KP_8] = "",
        [KEY_KP_9] = "",
        [KEY_KP_0] = "",
        [KEY_KP_PERIOD] = "",

        [KEY_NONUSBACKSLASH] = "",
        [KEY_APPLICATION] = "",
        [KEY_POWER] = "",
        [KEY_KP_EQUALS] = "",
        [KEY_F13] = "f13",
        [KEY_F14] = "f14",
        [KEY_F15] = "f15",
        [KEY_F16] = "f16",
        [KEY_F17] = "f17",
        [KEY_F18] = "f18",
        [KEY_F19] = "f19",
        [KEY_F20] = "f20",
        [KEY_F21] = "f21",
        [KEY_F22] = "f22",
        [KEY_F23] = "f23",
        [KEY_F24] = "f24",
        [KEY_EXECUTE] = "",
        [KEY_HELP] = "",
        [KEY_MENU] = "",
        [KEY_SELECT] = "",
        [KEY_STOP] = "",
        [KEY_AGAIN] = "",
        [KEY_UNDO] = "",
        [KEY_CUT] = "",
        [KEY_COPY] = "",
        [KEY_PASTE] = "",
        [KEY_FIND] = "",
        [KEY_MUTE] = "",
        [KEY_VOLUMEUP] = "",
        [KEY_VOLUMEDOWN] = "",

        [KEY_KP_COMMA] = "",
        [KEY_KP_EQUALSAS400] = "",

        [KEY_INTERNATIONAL1] = "",
        [KEY_INTERNATIONAL2] = "",
        [KEY_INTERNATIONAL3] = "",
        [KEY_INTERNATIONAL4] = "",
        [KEY_INTERNATIONAL5] = "",
        [KEY_INTERNATIONAL6] = "",
        [KEY_INTERNATIONAL7] = "",
        [KEY_INTERNATIONAL8] = "",
        [KEY_INTERNATIONAL9] = "",
        [KEY_LANG1] = "",
        [KEY_LANG2] = "",
        [KEY_LANG3] = "",
        [KEY_LANG4] = "",
        [KEY_LANG5] = "",
        [KEY_LANG6] = "",
        [KEY_LANG7] = "",
        [KEY_LANG8] = "",
        [KEY_LANG9] = "",

        [KEY_ALTERASE] = "",
        [KEY_SYSREQ] = "",
        [KEY_CANCEL] = "",
        [KEY_CLEAR] = "",
        [KEY_PRIOR] = "",
        [KEY_RETURN2] = "",
        [KEY_SEPARATOR] = "",
        [KEY_OUT] = "",
        [KEY_OPER] = "",
        [KEY_CLEARAGAIN] = "",
        [KEY_CRSEL] = "",
        [KEY_EXSEL] = "",

        [KEY_KP_00] = "",
        [KEY_KP_000] = "",
        [KEY_THOUSANDSSEPARATOR] = "",
        [KEY_DECIMALSEPARATOR] = "",
        [KEY_CURRENCYUNIT] = "",
        [KEY_CURRENCYSUBUNIT] = "",
        [KEY_KP_LEFTPAREN] = "",
        [KEY_KP_RIGHTPAREN] = "",
        [KEY_KP_LEFTBRACE] = "",
        [KEY_KP_RIGHTBRACE] = "",
        [KEY_KP_TAB] = "",
        [KEY_KP_BACKSPACE] = "",
        [KEY_KP_A] = "",
        [KEY_KP_B] = "",
        [KEY_KP_C] = "",
        [KEY_KP_D] = "",
        [KEY_KP_E] = "",
        [KEY_KP_F] = "",
        [KEY_KP_XOR] = "",
        [KEY_KP_POWER] = "",
        [KEY_KP_PERCENT] = "",
        [KEY_KP_LESS] = "",
        [KEY_KP_GREATER] = "",
        [KEY_KP_AMPERSAND] = "",
        [KEY_KP_DBLAMPERSAND] = "",
        [KEY_KP_VERTICALBAR] = "",
        [KEY_KP_DBLVERTICALBAR] = "",
        [KEY_KP_COLON] = "",
        [KEY_KP_HASH] = "",
        [KEY_KP_SPACE] = "",
        [KEY_KP_AT] = "",
        [KEY_KP_EXCLAM] = "",
        [KEY_KP_MEMSTORE] = "",
        [KEY_KP_MEMRECALL] = "",
        [KEY_KP_MEMCLEAR] = "",
        [KEY_KP_MEMADD] = "",
        [KEY_KP_MEMSUBTRACT] = "",
        [KEY_KP_MEMMULTIPLY] = "",
        [KEY_KP_MEMDIVIDE] = "",
        [KEY_KP_PLUSMINUS] = "",
        [KEY_KP_CLEAR] = "",
        [KEY_KP_CLEARENTRY] = "",
        [KEY_KP_BINARY] = "",
        [KEY_KP_OCTAL] = "",
        [KEY_KP_DECIMAL] = "",
        [KEY_KP_HEXADECIMAL] = "",

        [KEY_LCTRL] = "",
        [KEY_LSHIFT] = "",
        [KEY_LALT] = "",
        [KEY_LGUI] = "",
        [KEY_RCTRL] = "",
        [KEY_RSHIFT] = "",
        [KEY_RALT] = "",
        [KEY_RGUI] = "",

        [KEY_MODE] = "",

        [KEY_AUDIONEXT] = "",
        [KEY_AUDIOPREV] = "",
        [KEY_AUDIOSTOP] = "",
        [KEY_AUDIOPLAY] = "",
        [KEY_AUDIOMUTE] = "",
        [KEY_MEDIASELECT] = "",
        [KEY_WWW] = "",
        [KEY_MAIL] = "",
        [KEY_CALCULATOR] = "",
        [KEY_COMPUTER] = "",
        [KEY_AC_SEARCH] = "",
        [KEY_AC_HOME] = "",
        [KEY_AC_BACK] = "",
        [KEY_AC_FORWARD] = "",
        [KEY_AC_STOP] = "",
        [KEY_AC_REFRESH] = "",
        [KEY_AC_BOOKMARKS] = "",

        [KEY_BRIGHTNESSDOWN] = "",
        [KEY_BRIGHTNESSUP] = "",
        [KEY_DISPLAYSWITCH] = "",
        [KEY_KBDILLUMTOGGLE] = "",
        [KEY_KBDILLUMDOWN] = "",
        [KEY_KBDILLUMUP] = "",
        [KEY_EJECT] = "",
        [KEY_SLEEP] = "",

        [KEY_APP1] = "",
        [KEY_APP2] = "",
};