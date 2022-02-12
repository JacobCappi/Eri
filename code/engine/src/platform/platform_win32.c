#include "platform/platform.h"

// TODO: Refactor this. Can't without windows libs on my linux system
// I don't think I should use mem.h, since mem.h just calls platform memory 
// I am forfeiting memory tracking though
#if ERI_PLATFORM_WINDOWS

#include "core/logger.h"
#include "core/input.h"

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>

struct win32_state
{
    HINSTANCE h_instance;
    HWND h_wnd;
};


static f64 clock_frequency;      // Multiplier for clock cycles to get seconds
static LARGE_INTEGER start_time; // Starting time of the application

LRESULT CALLBACK wnd_msg_handler(HWND h_wnd, u32 message, WPARAM w_param, LPARAM l_param);
enum keyboard translate_key(u16 keys);

b8 init_platform(
    struct platform_state *state,
    const char *name,
    i32 x, i32 y,
    i32 width, i32 height
)
{
    state->os_specific_state = platform_malloc(sizeof(struct win32_state), FALSE);
    struct win32_state *os_state = (struct win32_state *)state->os_specific_state;

    os_state->h_instance = GetModuleHandleA(NULL);
    const char* window_class_name = "eri_window_class";

    // Creating the Window Class [WNDCLASSEX]
    WNDCLASSEXA wnd_class;
    platform_memzero(&wnd_class, sizeof(wnd_class));
    wnd_class.cbSize = sizeof(wnd_class);
    wnd_class.style = CS_DBLCLKS;
    wnd_class.lpfnWndProc = wnd_msg_handler; // LRESULT (CALLBACK* WNDPROC) (HWND, UINT, WPARAM, LPARAM); 
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hInstance = os_state->h_instance;
    wnd_class.hIcon = LoadIcon(os_state->h_instance, IDI_APPLICATION);
    wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd_class.hbrBackground = NULL;
    wnd_class.lpszClassName = window_class_name;
    wnd_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // Register that window class
    if(!RegisterClassEx(&wnd_class))
    {
        MessageBoxA(0, "[ Failed ] Window Class Register", "ERROR", MB_ICONEXCLAMATION | MB_OK);
        ERI_LOG_FATAL("Window Class did not register");
        return FALSE;
    }

    // Sizes x,y and width,height from params 

    /*
        NOTE: Something about windowing:
        The Window is the size of the entire window,
        and the Client Zone, is the area dedicated to the client.
        (ie. excluding the border and the toolbar, etc...)

        My x,y,width,hieght is expected to be the client zone size.
    */

    u32 window[4] = { x, y, width, height };

    // Border and Button styling for Window: Styles are found in the MSDN
    // These are the ones KOHI engine thought were good, and honestly they look nice
    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    u32 window_ex_style = WS_EX_APPWINDOW;

    // Calculating the size of the styling to add to border size
    RECT border = { 0, 0, 0, 0 };
    AdjustWindowRectEx(&border, window_style, 0, window_ex_style);

    window[0] += border.left;
    window[1] += border.top;
    window[2] += border.right - border.left;
    window[3] += border.bottom - border.top;

    HWND window_handle = CreateWindowExA(
        window_ex_style, window_class_name, name, window_style,
        window[0], window[1], window[2], window[3], // x, y, width, height
        0, 0, os_state->h_instance, 0);

    if( !window_handle )
    {
        MessageBoxA(0, "[ Failed ] Window Creation", "ERROR", MB_ICONEXCLAMATION | MB_OK);
        ERI_LOG_FATAL("Window failed to create");
        return FALSE;
    }
    else
    {
        os_state->h_wnd = window_handle;
    }

    // Show the window
    b32 window_active = 1;  // TODO: if the window should not accept input, this should be false.
    i32 show_window_command_flags = window_active ? SW_SHOW : SW_SHOWNOACTIVATE;
    // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
    // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE
    ShowWindow(os_state->h_wnd, show_window_command_flags);


    // Kohi Absolute time continued...
    // 1/frequency for the frequency*sec to get sec.
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return TRUE;
}

void shutdown_platform(struct platform_state *state)
{
    // Simply cold-cast to the known type.
    struct win32_state *os_state = (struct win32_state *)state->os_specific_state;

    if (os_state->h_wnd) 
    {
        DestroyWindow(os_state->h_wnd);
        os_state->h_wnd = 0;
    }
}

b8 platform_message(struct platform_state *state)
{
    // Peek Message overwrites MSG values, so needs a hard instantiation
    // Windows is event based, so these 'messages' are things like key presses, etc...
    MSG message;

    // NULL value for handle receives all messages on thread or on null handles
    // this is in a while loop b/c thread is locked until stack is cleared to my understanding
    // ALSO note, peek is used, as getMessage will lock until a message is received
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) 
    {
        TranslateMessage(&message);

        // Dispatch fires the callback set during window creation 
        DispatchMessageA(&message);
    }
    return TRUE;
}

// TODO: Memory systems: currently temp use of stdlib
void *platform_malloc(u64 size, b8 aligned)
{
    return malloc(size);
}

void platform_free(void* memory, b8 aligned)
{
    free(memory);
}

void *platform_memzero(void *memory, u64 size)
{
    return memset(memory, 0, size);
}

void *platform_memcpy(void *destination, const void *source, u64 size)
{
    return memcpy(destination, source, size);
}

void *platform_memset(void *memory, i32 value, u64 size)
{
    return memset(memory, value, size);
}

void platform_cout(const char *text, u8 color)
{
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD dwMode = 0;
    if (!GetConsoleMode(h_console, &dwMode))
    {
        ERI_LOG_WARNING("Failed to set Console Mode, output is without color");
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(h_console, dwMode))
    {
        ERI_LOG_WARNING("Failed to set Console Mode, output is without color");
    }

    // Fatal, Error, Warning, Info, Debug, Trace
    const char* color_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", color_strings[color], text);
}

void platform_cerr(const char *text, u8 color)
{
    HANDLE h_console = GetStdHandle(STD_ERROR_HANDLE);

    DWORD dwMode = 0;
    if (!GetConsoleMode(h_console, &dwMode))
    {
        ERI_LOG_WARNING("Failed to set Console Mode, output is without color");
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(h_console, dwMode))
    {
        ERI_LOG_WARNING("Failed to set Console Mode, output is without color");
    }

    // Fatal, Error, Warning, Info, Debug, Trace
    const char* color_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", color_strings[color], text);
}

f64 platform_time(void)
{
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    // Number of cycles since application started * that multiplier
    // for seconds the application has been on
    return (f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms)
{
    // Windows has a Sleep function, so that will be used
    Sleep(ms);
}

LRESULT CALLBACK wnd_msg_handler(HWND h_wnd, u32 message, WPARAM w_param, LPARAM l_param)
{
    switch(message)
    {
        case WM_ERASEBKGND:
            // MSDN return value for differing this to us [Vulkan]
            return 1;

        case WM_CLOSE:
            // TODO: Fire an event for the application to quit after building event system
            return 0; 

        case WM_DESTROY:
            // Posts WM_QUIT
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
        {
            // TODO: Renderer will handle resize
        } break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: 
        {
            b8 is_pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
            u16 win_key = (u16) w_param;
            enum keyboard key = translate_key(win_key);
            
            input_handle_keyboard(key, is_pressed);
        } break;

        case WM_MOUSEMOVE: {
            i32 mouse_x = GET_X_LPARAM(l_param);
            i32 mouse_y = GET_Y_LPARAM(l_param);

            input_handle_mouse_xy(mouse_x, mouse_y);
        } break;

        case WM_MOUSEWHEEL: {
            i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            if (z_delta != 0) {
                // Flatten the input to an OS-independent (-1, 1)
                z_delta = (z_delta < 0) ? -1 : 1;
                input_handle_mouse_wheel(z_delta);
            }
        } break;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            b8 is_pressed = (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN);
            enum mouse button = MAX_MOUSE_BUTTONS;
            switch (message) 
            {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    button = MOUSE_LEFT;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    button = MOUSE_MID;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    button = MOUSE_RIGHT;
                    break;
            }
            if (button != MAX_MOUSE_BUTTONS)
            {
                input_handle_mouse(button, is_pressed);
            }

        } break;
    }

    // Default processing on any event that we are not supporting
    // ie, Windows will do it
    return DefWindowProcA(h_wnd, message, w_param, l_param);
}

enum keyboard translate_key(u16 keys)
{
    // This took another hour.... respect it
    switch (keys)
    {
        case VK_BACK:
            return KEY_BACKSPACE;
        case VK_TAB:
            return KEY_TAB;
        case VK_RETURN:
            return KEY_ENTER;
        case VK_SHIFT:
            return KEY_SHIFT;
        case VK_CONTROL:
            return KEY_CONTROL;
        case VK_MENU:
            return KEY_ALT;
        case VK_PAUSE:
            return KEY_PAUSE;
        case VK_CAPITAL:
            return KEY_CAPSLK;
    // This should just be every language button
        case VK_IME_ON:
        case VK_HANGUL:
        case VK_JUNJA:
        case VK_FINAL:
        case VK_HANJA:
        case VK_IME_OFF:
            ERI_LOG_WARNING("I don't support these");
            return KEY_IME_LANG;
        case VK_ESCAPE:
            return KEY_ESCAPE;
        case VK_SPACE:
            return KEY_SPACE;
        case VK_PRIOR:
            return KEY_PGUP;
        case VK_NEXT:
            return KEY_PGDWN;
        case VK_END:
            return KEY_END;
        case VK_HOME:
            return KEY_HOME;
        case VK_LEFT:
            return KEY_LEFT;
        case VK_UP:
            return KEY_UP;
        case VK_RIGHT:
            return KEY_RIGHT;
        case VK_DOWN:
            return KEY_DOWN;
        case VK_SELECT:
            return KEY_SELECT;
        case VK_PRINT:
            return KEY_PRINT;
    // Not sure if this is legacy, but converting to enter
        case VK_EXECUTE:
            return KEY_ENTER;
        case VK_SNAPSHOT:
            return KEY_PRINT;
        case VK_INSERT:
            return KEY_INSERT;
        case VK_DELETE:
            return KEY_DELETE;
    // API doesn't define these, so raw hex for these....
        case 0x30:
            return KEY_0;
        case 0x31:
            return KEY_1;
        case 0x32:
            return KEY_2;
        case 0x33:
            return KEY_3;
        case 0x34:
            return KEY_4;
        case 0x35:
            return KEY_5;
        case 0x36:
            return KEY_6;
        case 0x37:
            return KEY_7;
        case 0x38:
            return KEY_8;
        case 0x39:
            return KEY_9;
        case 0x41:
            return KEY_A;
        case 0x42:
            return KEY_B;
        case 0x43:
            return KEY_C;
        case 0x44:
            return KEY_D;
        case 0x45:
            return KEY_E;
        case 0x46:
            return KEY_F;
        case 0x47:
            return KEY_G;
        case 0x48:
            return KEY_H;
        case 0x49:
            return KEY_I;
        case 0x4A:
            return KEY_J;
        case 0x4B:
            return KEY_K;
        case 0x4C:
            return KEY_L;
        case 0x4D:
            return KEY_M;
        case 0x4E:
            return KEY_N;
        case 0x4F:
            return KEY_O;
        case 0x50:
            return KEY_O;
        case 0x51:
            return KEY_Q;
        case 0x52:
            return KEY_R;
        case 0x53:
            return KEY_S;
        case 0x54:
            return KEY_T;
        case 0x55:
            return KEY_U;
        case 0x56:
            return KEY_V;
        case 0x57:
            return KEY_W;
        case 0x58:
            return KEY_X;
        case 0x59:
            return KEY_Y;
        case 0x5A:
            return KEY_Z;
        case VK_LWIN:
            return KEY_LSUPER;
        case VK_RWIN:
            return KEY_RSUPER;
        case VK_NUMPAD0:
            return KEY_KP_0;
        case VK_NUMPAD1:
            return KEY_KP_1;
        case VK_NUMPAD2:
            return KEY_KP_2;
        case VK_NUMPAD3:
            return KEY_KP_3;
        case VK_NUMPAD4:
            return KEY_KP_4;
        case VK_NUMPAD5:
            return KEY_KP_5;
        case VK_NUMPAD6:
            return KEY_KP_6;
        case VK_NUMPAD7:
            return KEY_KP_7;
        case VK_NUMPAD8:
            return KEY_KP_8;
        case VK_NUMPAD9:
            return KEY_KP_9;
        case VK_MULTIPLY:
            return KEY_KP_MULT;
        case VK_ADD:
            return KEY_KP_ADD;
        case VK_SUBTRACT:
            return KEY_KP_SUB;
        case VK_DECIMAL:
            return KEY_KP_DOT;
        case VK_DIVIDE:
            return KEY_KP_DIV;
        case VK_F1:
            return KEY_F1;
        case VK_F2:
            return KEY_F2;
        case VK_F3:
            return KEY_F3;
        case VK_F4:
            return KEY_F4;
        case VK_F5:
            return KEY_F5;
        case VK_F6:
            return KEY_F6;
        case VK_F7:
            return KEY_F7;
        case VK_F8:
            return KEY_F8;
        case VK_F9:
            return KEY_F9;
        case VK_F10:
            return KEY_F10;
        case VK_F11:
            return KEY_F11;
        case VK_F12:
            return KEY_F12;
        case VK_F13:
            return KEY_F13;
        case VK_F14:
            return KEY_F14;
        case VK_F15:
            return KEY_F15;
        case VK_F16:
            return KEY_F16;
        case VK_F17:
            return KEY_F17;
        case VK_F18:
            return KEY_F18;
        case VK_F19:
            return KEY_F19;
        case VK_F20:
            return KEY_F20;
        case VK_F21:
            return KEY_F21;
        case VK_F22:
            return KEY_F22;
        case VK_F23:
            return KEY_F23;
        case VK_F24:
            return KEY_F24;
        case VK_NUMLOCK:
            return KEY_NUMLK;
        case VK_SCROLL:
            return KEY_SCRLLK;
        case VK_LSHIFT:
            return KEY_LSHIFT;
        case VK_RSHIFT:
            return KEY_RSHIFT;
        case VK_LCONTROL:
            return KEY_LCONTROL;
        case VK_RCONTROL:
            return KEY_RCONTROL;
        case VK_LMENU:
            return KEY_LMENU;
        case VK_RMENU:
            return KEY_RMENU;

        case VK_OEM_1:
            return KEY_SEMICOLON;
        case VK_OEM_PLUS:
            return KEY_ADD;
        case VK_OEM_COMMA:
            return KEY_COMMA;
        case VK_OEM_MINUS:
            return KEY_MINUS;
        case VK_OEM_PERIOD:
            return KEY_PERIOD;
        case VK_OEM_2:
            return KEY_FSLASH;
        case VK_OEM_3:
            return KEY_TILD;
        case VK_OEM_4:
            return KEY_LBRACKET;
        case VK_OEM_5:
            return KEY_BSLASH;
        case VK_OEM_6:
            return KEY_RBRACKET;
        case VK_OEM_7:
            return KEY_QUOTE;

        default:
            return 0;
    }
}

#endif
