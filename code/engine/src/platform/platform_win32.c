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
    platform_state *state,
    const char *name,
    i32 x, i32 y,
    i32 width, i32 height
)
{
    state->os_specific_state = platform_malloc(sizeof(win32_state), FALSE);
    struct win32_state *os_state = (win32_state *)state->os_specific_state;

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

void platform_shutdown(platform_state *state)
{
    // Simply cold-cast to the known type.
    win32_state *os_state = (win32_state *)state->os_specific_state;

    if (os_state->h_wnd) 
    {
        DestroyWindow(os_state->h_wnd);
        os_state->h_wnd = 0;
    }
}

b8 platform_message(platform_state *state)
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
            b8 is_pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            u16 win_key = (u16) w_param;
            enum keyboard key = translate_key(keys);
            
            input_handle_keyboard(key, is_pressed);
        } break;

        case WM_MOUSEMOVE: {
            i32 mouse_x = GET_X_PARAM(l_param);
            i32 mouse_y = GET_X_PARAM(l_param);

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
            enum mouse button = MAX_MOUSE_BUTTON;
            switch (message) 
            {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    button = MOUSE_LEFT;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    button = MOUSE_MIDDLE;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    button = MOUSE_RIGHT;
                    break;
            }
            if (button != MAX_MOUSE_BUTTON)
            {
                input_handle_mouse(button);
            }

        } break;
    }

    // Default processing on any event that we are not supporting
    // ie, Windows will do it
    return DefWindowProcA(h_wnd, message, w_param, l_param);
}

enum keyboard translate_key(u16 keys)
{
    // TODO: do this once I get on a windows system
}

#endif