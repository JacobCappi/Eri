#pragma once

#include "Core.h"
#include "Subsystems/Platform/IPlatform.h"
#include "Subsystems/Logger/ILogger.h"
#include "Subsystems/Events/IEvents.h"
#include "Subsystems/Platform/Windows/WindowKeys.h"

#include <windows.h>
#include <windowsx.h>

namespace ERI
{

class PlatformWindows : public IPlatform
{
private:
    i32 _windowX;
    i32 _windowY;
    i32 _windowWidth;
    i32 _windowHeight;

private:
    LARGE_INTEGER _lpPerformanceCounter;
    LARGE_INTEGER _clock_ns;

    HANDLE _timer;
    f64 _clock_frequency; // clock ticks / ms
    i64 _current_clock; // ticks

private:
    ILogger *_log;
    IEvents *_events;

private:
    HWND _window; // This is a handle to the window. Typedef hides the pointer
    static LRESULT CALLBACK MessageCallback(HWND hWnd, u32 uMsg, WPARAM wParam, LPARAM lParam);

public:
    PlatformWindows() {};
    ~PlatformWindows() {};

public: // ISubsystem
    bool Startup() override;
    bool Shutdown() override;

public: // IPlatform
    bool getPlatformMessage() override;
    void SetWindowSize(i32 width, i32 height) override;
    void SetWindowPosition(i32 x, i32 y) override;
    bool StartupWindow(const char *windowName) override;

    void registerLogger(ILogger *log) override;
    void registerEvents(IEvents *events) override;

    void clock_start() override;
    f64 clock_delta() override;
    void sleep(u64) override;

    LRESULT ProcessWindowsMessage(HWND hWnd, u32 uMsg, WPARAM wParam, LPARAM lParam);
};
    
} // namespace ERI