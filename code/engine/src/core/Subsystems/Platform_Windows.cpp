#include "core/Subsystems/Platform.h"

//TODO: Consider interfacing? Problem is, preprocessor might be neccassary
// b/c windows sdk can't be compiled on any other OS
#if ERI_PLATFORM_WINDOWS

#include <windows.h>
#include <windowsx.h>
#include <iostream>

using namespace ERI
{
    struct win32_windowing
    {
        HINSTANCE instance;
        HWND wnd;
    };

    // Find the 'C++' way to do this? where do I register callbacks in C++ other than in my namespace
    LRESULT CALLBACK wnd_msg_handler(HWND h_wnd, u32 message, WPARAM w_param, LPARAM l_param);

    b8 Platform::init(Logger *logger)
    {
        log = logger;
        return (log == nullptr) ? FALSE : TRUE;
    }

    void Platform::shutdown()
    {
        struct win32_windowing *state = (struct win32_windowing *)abstract_wnd_state;
        if (state)
        {
            DestroyWindow(state->wnd);
            state->wnd = nullptr;
        }
    }

    void Platform::print_name(std::ostream& str) const
    {
        str << "Windows Platform";
    }


    b8 Platform::init_windowing(const char *wnd_name, i32 x, i32 y, u32 width, u32 height)
    {
        abstract_wnd_state = malloc(sizeof(struct win32_windowing), FALSE);
        struct win32_windowing *state = (struct win32_windowing *)abstract_wnd_state;

        state->instance = GetModuleHandleA(NULL);

        // WNDCLASSEXA needs this info for id(?) for that handle later
        WNDCLASSEXA wnd_class;
        memzero(&wnd_class, sizeof(wnd_class));
        const char *wnd_class_name = "ERI_WIN_CLASS";

        wnd_class.cbSize = sizeof(wnd_class);
        wnd_class.style = CS_DBLCLKS;
        wnd_class.lpfnWndProc = wnd_msg_handler; // LRESULT (CALLBACK* WNDPROC) (HWND, UINT, WPARAM, LPARAM); 
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hInstance = state->instance;
        wnd_class.hIcon = LoadIcon(state->instance, IDI_APPLICATION);
        wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
        wnd_class.hbrBackground = NULL;
        wnd_class.lpszClassName = wnd_class_name;
        wnd_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if(!RegisterClassExA(&wnd_class))
        {
            std::cout << "Window Class did not register";
            return FALSE;
        }

        // C++ taking types *very* seriously
        u32 wnd_data[4] = { static_cast<u32>(x), static_cast<u32>(y), width, height};

        u32 wnd_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        wnd_style |= WS_MAXIMIZEBOX;
        wnd_style |= WS_MINIMIZEBOX;
        wnd_style |= WS_THICKFRAME;

        u32 wnd_ex_style = WS_EX_APPWINDOW;

        // Getting our sizes
        RECT border = { 0, 0, 0, 0 };
        AdjustWindowRectEx(&border, wnd_style, 0, wnd_ex_style);
        wnd_data[0] += border.left;
        wnd_data[1] += border.top;
        wnd_data[2] += border.right - border.left;
        wnd_data[3] += border.bottom - border.top;


        HWND wnd_handle = CreateWindowExA(
            wnd_ex_style, wnd_class_name, wnd_name, wnd_style,
            wnd_data[0], wnd_data[1], wnd_data[2], wnd_data[3], // x, y, width, height
            0, 0, state->instance, 0);

        if (!wnd_handle)
        {
            std::cout << "Invalid Handle";
            return FALSE;
        }
        else
        {
            state->wnd = wnd_handle;
        }

        b32 window_active = 1;  // TODO: if the window should not accept input, this should be false.
        i32 show_window_command_flags = window_active ? SW_SHOW : SW_SHOWNOACTIVATE;
        // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
        // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE
        ShowWindow(state->wnd, show_window_command_flags);


        // Kohi Absolute time continued...
        // 1/frequency for the frequency*sec to get sec.
        // TODO: C++ has to have a faster/better way for time ....
        return TRUE;
    }

    b8 Platform::pump_message()
    {
        // Uses the callback set at wnd creation
        MSG message;

        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        return TRUE;
    }

    // TODO: Eventually get some memory subsystem upgrades
    // Handle allignment
    void *Platform::malloc(u64 sz, b8 alligned)
    {
        return std::malloc(sz);
    }

    void Platform::free(void *memory, b8 alligned)
    {
        std::free(memory);
    }
    
    void *Platform::memcpy(void *dest, const void *src, u64 sz)
    {
        return std::memcpy(dest, src, sz);
    }

    void *Platform::memzero(void *memory, u64 sz)
    {
        return std::memset(memory, 0, sz);

    }

    void *Platform::memset(void *memory, i32 value, u64 sz)
    {
        return std::memset(memory, value, sz);
    }

    void Platform::sleep(u64 ms)
    {
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
                /*
                b8 is_pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
                u16 win_key = (u16) w_param;
                enum keyboard key = translate_key(win_key);
                
                input_handle_keyboard(key, is_pressed);
                */
            } break;

            case WM_MOUSEMOVE: {
                /*
                i32 mouse_x = GET_X_LPARAM(l_param);
                i32 mouse_y = GET_Y_LPARAM(l_param);

                input_handle_mouse_xy(mouse_x, mouse_y);
                */
            } break;

            case WM_MOUSEWHEEL: {
                /*
                i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
                if (z_delta != 0) {
                    // Flatten the input to an OS-independent (-1, 1)
                    z_delta = (z_delta < 0) ? -1 : 1;
                    input_handle_mouse_wheel(z_delta);
                }
                */
            } break;

            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: {
                /*
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
                */

            } break;
        }

        // Default processing on any event that we are not supporting
        // ie, Windows will do it
        return DefWindowProcA(h_wnd, message, w_param, l_param);
    }

}

#endif