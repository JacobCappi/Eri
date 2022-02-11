#pragma once

#include "defines.h"

enum mouse
{
    MOUSE_LEFT    = 0x01,
    MOUSE_MID     = 0x02,
    MOUSE_RIGHT   = 0x03,

// Currently up to 4 extra buttons
// Most should only use 2
    MOUSE_X1      = 0x04,
    MOUSE_X2      = 0x05,
    MOUSE_X3      = 0x06,
    MOUSE_X4      = 0x07,

    MAX_MOUSE_BUTTONS,
};

/*
    Anything that can one-to-one with ascii follows ascii
    Keypad on 0x6
    Function on 0x7 and 0x8
    Arrow Keys and Locks on 0xA
    Extra Utility Keys on 0xB
    IME keys on 0xF (for foreign [international?])
*/
enum keyboard
{

    KEY_BACKSPACE = 0x08,
    KEY_TAB       = 0x09,
    KEY_ENTER     = 0x0D,

    KEY_SHIFT     = 0x10,
    KEY_LSHIFT    = 0x11,
    KEY_RSHIFT    = 0x12,

    KEY_CONTROL   = 0x13,
    KEY_LCONTROL  = 0x14,
    KEY_RCONTROL  = 0x15,

    KEY_ALT       = 0x16,
    KEY_LALT      = 0x17,
    KEY_RALT      = 0x18,

    KEY_LSUPER    = 0x19,
    KEY_RSUPER    = 0x1A,
    KEY_ESCAPE    = 0x1B,

    KEY_LMENU     = 0x1C,
    KEY_RMENU     = 0x1D,
    KEY_LLANG     = 0x1E,
    KEY_RLANG     = 0x1F,

    KEY_SPACE     = 0x20,
    KEY_EXCLAM    = 0x21,
    KEY_DBLQT     = 0x22,
    KEY_POUND     = 0x23,
    KEY_MNYSN     = 0x24,
    KEY_PERCENT   = 0x25,
    KEY_AMPERSAN  = 0x26,
    KEY_QUOTE     = 0x27,
    KEY_LPAREN    = 0x28,
    KEY_RPAREN    = 0x29,
    KEY_MULT      = 0x2A,
    KEY_ADD       = 0x2B,
    KEY_COMMA     = 0x2C,
    KEY_MINUS     = 0x2D,
    KEY_PERIOD    = 0x2E,
    KEY_FSLASH    = 0x2F,

    KEY_0         = 0x30,
    KEY_1         = 0x31,
    KEY_2         = 0x32,
    KEY_3         = 0x33,
    KEY_4         = 0x34,
    KEY_5         = 0x35,
    KEY_6         = 0x36,
    KEY_7         = 0x37,
    KEY_8         = 0x38,
    KEY_9         = 0x39,
    KEY_COLON     = 0x3A,
    KEY_SEMICOLON = 0x3B,
    KEY_LT        = 0x3C,
    KEY_EQ        = 0x3D,
    KEY_GT        = 0x3C,
    KEY_QUESTION  = 0x3F,
    KEY_AT        = 0x40,

    KEY_A         = 0x41,
    KEY_B         = 0x42,
    KEY_C         = 0x43,
    KEY_D         = 0x44,
    KEY_E         = 0x45,
    KEY_F         = 0x46,
    KEY_G         = 0x47,
    KEY_H         = 0x48,
    KEY_I         = 0x49,
    KEY_J         = 0x4A,
    KEY_K         = 0x4B,
    KEY_L         = 0x4C,
    KEY_M         = 0x4D,
    KEY_N         = 0x4E,
    KEY_O         = 0x4F,
    KEY_P         = 0x50,
    KEY_Q         = 0x51,
    KEY_R         = 0x52,
    KEY_S         = 0x53,
    KEY_T         = 0x54,
    KEY_U         = 0x55,
    KEY_V         = 0x56,
    KEY_W         = 0x57,
    KEY_X         = 0x58,
    KEY_Y         = 0x59,
    KEY_Z         = 0x5A,
    KEY_LBRACKET  = 0x5B,
    KEY_BSLASH    = 0x5C,
    KEY_RBRACKET  = 0x5D,
    KEY_CARROT    = 0x5E,
    KEY_UNDERSCR  = 0x5F,
    KEY_GRAVE     = 0x60,


    KEY_KP_0      = 0x70,
    KEY_KP_1      = 0x71,
    KEY_KP_2      = 0x72,
    KEY_KP_3      = 0x73,
    KEY_KP_4      = 0x74,
    KEY_KP_5      = 0x75,
    KEY_KP_6      = 0x76,
    KEY_KP_7      = 0x77,
    KEY_KP_8      = 0x78,
    KEY_KP_9      = 0x79,

    KEY_LBRACE    = 0x7B,
    KEY_BAR       = 0x7C,
    KEY_RBRACE    = 0x7D,
    KEY_TILD      = 0x7E,

    KEY_F1        = 0x80,
    KEY_F2        = 0x81,
    KEY_F3        = 0x82,
    KEY_F4        = 0x83,
    KEY_F5        = 0x84,
    KEY_F6        = 0x85,
    KEY_F7        = 0x86,
    KEY_F8        = 0x87,
    KEY_F9        = 0x88,
    KEY_F10       = 0x89,
    KEY_F11       = 0x8A,
    KEY_F12       = 0x8B,
    KEY_F13       = 0x8C,
    KEY_F14       = 0x8D,
    KEY_F15       = 0x8E,
    KEY_F16       = 0x8F,
    KEY_F17       = 0x90,
    KEY_F18       = 0x91,
    KEY_F19       = 0x92,
    KEY_F20       = 0x93,
    KEY_F21       = 0x94,
    KEY_F22       = 0x95,
    KEY_F23       = 0X96,
    KEY_F24       = 0x97,

    KEY_KP_DIV    = 0x9A,
    KEY_KP_MULT   = 0x9B,
    KEY_KP_SUB    = 0x9C,
    KEY_KP_ADD    = 0x9D,
    KEY_KP_DOT    = 0x9E,
    KEY_KP_ENTER  = 0x9F,

    KEY_LEFT      = 0xA0,
    KEY_UP        = 0xA1,
    KEY_RIGHT     = 0xA2,
    KEY_DOWN      = 0xA3,

    KEY_CAPSLK    = 0xAA,
    KEY_NUMLK     = 0xAB,
    KEY_SCRLLK    = 0xAC,

    KEY_PGUP      = 0xB1,
    KEY_PGDWN     = 0xB2,
    KEY_END       = 0xB3,
    KEY_HOME      = 0xB4,
    KEY_INSERT    = 0xB5,
    KEY_DELETE    = 0xB6,

    KEY_SELECT    = 0xB7,
    KEY_PRINT     = 0xB8,
    KEY_EXECUTE   = 0xB9,
    KEY_SNAPSHOT  = 0xBA,
    KEY_HELP      = 0xBB,
    KEY_PAUSE     = 0xBC,
    KEY_SLEEP     = 0xBD,

// IME Compatibility keys
    KEY_IME_CNVT  = 0xF0,
    KEY_IME_NCNVT = 0xF1,
    KEY_IME_ACCPT = 0xF2,
    KEY_IME_MDCHG = 0xF3,
    KEY_IME_LANG  = 0xF4,
    KEY_IME_PROC  = 0xF5,

    MAX_KEYS      = 0xFF
};

b8 init_input();
void shutdown_input();
void input_update(f64 delta_time);

ERI_API b8 input_is_key_down(enum keyboard key);
ERI_API b8 input_was_key_down(enum keyboard key);
ERI_API b8 input_is_key_up(enum keyboard key);
ERI_API b8 input_was_key_up(enum keyboard key);

ERI_API b8 input_is_button_down(enum mouse button);
ERI_API b8 input_was_button_down(enum mouse button);
ERI_API b8 input_is_button_up(enum mouse button);
ERI_API b8 input_was_button_up(enum mouse button);

ERI_API void input_new_mouse_xy(i32* x, i32* y);
ERI_API void input_old_mouse_xy(i32* x, i32* y);

void input_handle_keyboard(enum keyboard key, b8 pressed);
void input_handle_mouse(enum mouse button, b8 pressed);

void input_handle_mouse_xy(i16 x, i16 y);
void input_handle_mouse_wheel(i8 z_delta);
