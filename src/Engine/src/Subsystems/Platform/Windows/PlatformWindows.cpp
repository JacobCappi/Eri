#include "Subsystems/Platform/Windows/PlatformWindows.h"
#include "Subsystems/EventSystem/IEventSystem.h"

#include <windows.h>
#include <windowsx.h>

namespace ERI
{

bool PlatformWindows::Startup()
{
    _windowX = 0;
    _windowY = 0;
    _windowWidth = 0;
    _windowHeight = 0;

    if (_log == nullptr)
    {
        return false;
    }

    if (_events == nullptr)
    {
        _log->LogError("Windows Platform failed to register events");
        return false;
    }

    _window = nullptr;
    _log->LogInfo("Platform System type Window Setup");

    _wndKeys = new WindowKeys();

    return true;
}

bool PlatformWindows::Shutdown()
{
    if (_window == nullptr)
    {
        _log->LogInfo("Window was never created.");
        return true;
    }
    delete _wndKeys;
    return DestroyWindow(_window);
}


// https://stackoverflow.com/questions/34962000/understanding-different-strategies-for-message-handling-in-the-windows-api
// https://learn.microsoft.com/en-us/windows/win32/learnwin32/window-messages
bool PlatformWindows::getPlatformMessage()
{
    if(!_window)
    {
        _log->LogError("Window has not been initialized, cannot pump messages.");
        return false;
    }

    MSG message;
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return true;
}

void PlatformWindows::SetWindowSize(i32 width, i32 height)
{
    _windowWidth = width;
    _windowHeight = height;
}

void PlatformWindows::SetWindowPosition(i32 x, i32 y)
{
    _windowX = x;
    _windowY = y;
}

//https://learn.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
bool PlatformWindows::StartupWindow(const char *windowName)
{
    // Used to get icon/mouse from window libs
    auto moduleHandle = GetModuleHandleA(NULL);
    WNDCLASSEXA wndClass;

    wndClass.cbClsExtra = 0;
    wndClass.cbSize = sizeof(wndClass);
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = NULL;
    wndClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wndClass.hIcon = LoadIconA(moduleHandle, IDI_APPLICATION);
    wndClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    wndClass.hInstance = moduleHandle;
    wndClass.lpfnWndProc = MessageCallback;
    wndClass.lpszClassName = "EriWindow";
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_DBLCLKS;

    auto windowAtom = RegisterClassEx(&wndClass);
    if (windowAtom == 0)
    {
        DWORD errorID = GetLastError();
        if (errorID == 0)
        {
            _log->LogError("Failed to create window class. Reason unknown?");
        }
        else
        {
            LPSTR messageBuffer = nullptr;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
            _log->LogError("%s", messageBuffer);
        }
        return false;
    }

    // https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
    // https://social.msdn.microsoft.com/Forums/vstudio/en-US/a0f3d7e3-4290-4e99-970e-e75bf54ec65e/createwindow-width-and-height-problem?forum=vcgeneral
    // -> cause of an issue.... after checking mouse positions, the viewport ain't right.
    LPRECT box = new RECT{0,0,0,0};

    AdjustWindowRectEx(box, WS_OVERLAPPEDWINDOW, 0, WS_EX_APPWINDOW);
    _log->LogDebug("Box size offsets are, %d, %d, %d, %d", box->bottom, box->left, box->right, box->top); // bottom left right top is 8, -8, 8, -31 for this style
    i32 x = _windowX + box->left; // Scoots x by the border size so 0 is at client zone 0
    i32 y = _windowY + box->top; // Scoots y by the taskbar size so 0 is at client zone 0

    i32 width = _windowWidth + (box->right - box->left); // essentially extending the width to include the border
    i32 height = _windowHeight + (box->bottom - box->top);

    _window = CreateWindowExA(WS_EX_APPWINDOW, "EriWindow", windowName,
            WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, moduleHandle, this);

    if (_window == NULL)
    {
        DWORD errorID = GetLastError();
        if (errorID == 0)
        {
            _log->LogError("Failed to create window. Reason unknown?");
        }
        else
        {
            LPSTR messageBuffer = nullptr;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
            _log->LogError("%s", messageBuffer);
        }
        return false;
    }

    // When we get INI files, then this can be based on fullscreen or not
    ShowWindow(_window, SW_SHOW);
    return true;
}

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/writing-the-window-procedure
// Doc recommends throwing lengthy processes into another thread as this method blocks UI thread
LRESULT PlatformWindows::ProcessWindowsMessage(HWND hWnd, u32 uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            _events->PublishInternal(Internal::AppQuit, 0, 0);
            // TODO: Handle it myself
            return DefWindowProcA(hWnd, uMsg, wParam, lParam);
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        // These are Mouse Events
        // https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove
        case WM_MOUSEMOVE:
            SetFocus(_window);
            _events->PublishMouse(Mouse::Move, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        // https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-mousewheel
        case WM_MOUSEWHEEL:
        {
            i16 zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            // is a zdelta of 0 ub?
            if (zDelta > 0)
            {
                _events->PublishMouse(Mouse::ScrollUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            else if (zDelta < 0)
            {
                _events->PublishMouse(Mouse::ScrollDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            break;
        }
        case WM_LBUTTONDOWN:
            _events->PublishMouse(Mouse::LeftDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        case WM_RBUTTONDOWN:
            _events->PublishMouse(Mouse::RightDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        case WM_MBUTTONDOWN:
            _events->PublishMouse(Mouse::MiddleDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        case WM_LBUTTONUP:
            _events->PublishMouse(Mouse::LeftUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        case WM_RBUTTONUP:
            _events->PublishMouse(Mouse::RightUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        case WM_MBUTTONUP:
            _events->PublishMouse(Mouse::MiddleUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        // https://learn.microsoft.com/en-us/windows/win32/inputdev/mouse-input
        // These are Keypress events
        case WM_SIZE:
            // (0,0) -> *(lparam, lparam)
            // TODO: wparam has the 'max/min information... do I need that for fullscreen stuff?
            _log->LogDebug("Resize from Windows, new sizes (width: %d, height: %d)", LOWORD(lParam), HIWORD(lParam));
            _events->PublishInternal(Internal::WindowResize, LOWORD(lParam), HIWORD(lParam));
            break;
        //https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            _events->PublishKeyPress(_wndKeys->translateKey(static_cast<u16>(wParam)), KeyPress::DOWN);
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            _events->PublishKeyPress(_wndKeys->translateKey(static_cast<u16>(wParam)), KeyPress::UP);
            break;
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

void PlatformWindows::registerLogger(ILogger *log)
{
    _log = log;

}

void PlatformWindows::registerEvents(IEventSystem *events)
{
    _events = events;
}

// https://devblogs.microsoft.com/oldnewthing/20140203-00/?p=1893
// A way to store the pointer to this class's object so I can fetch it in the static callback
LRESULT CALLBACK PlatformWindows::MessageCallback(HWND hWnd, u32 uMsg, WPARAM wParam, LPARAM lParam)
{
    PlatformWindows *ourInstance;

    // Create our Platform instance pointer
    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        ourInstance = static_cast<PlatformWindows*>(lpcs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ourInstance));
    }
    else
    {
        ourInstance = reinterpret_cast<PlatformWindows*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (ourInstance != NULL)
    {
        return ourInstance->ProcessWindowsMessage(hWnd, uMsg, wParam, lParam);
    }
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

} // namespace ERI