#pragma once
#include "Core.h"

#include "Subsystems/Subsystems.h"

#include "Subsystems/Events/KeyEvents/IKeyEventSubscriber.h"
#include "Subsystems/Events/MouseEvents/IMouseEventSubscriber.h"
#include "Subsystems/Events/WindowEvents/IWindowStateSubscriber.h"

namespace ERI
{
    /**
     * @brief Main Tester App for ERI
     * @implements
     * @implements
     * @implements
     */
    class ERI_API App :
        public IKeyEventSubscriber,
        public IMouseEventSubscriber,
        public IWindowStateSubscriber
    {
    private:
        bool _isRunning;
        const f64 _fps = 30.0;
        const f64 _time_per_frame = (1.0 / _fps) * 1000;

        f64 _frame_time = 0;

        ILogger *_log;

    private:
        i32 _x_pos;
        i32 _y_pos;
        i32 _width;
        i32 _height;
        const char *_app_name;

    public:
        App(i32 x, i32 y, i32 width, i32 height, const char *app_name);
        virtual ~App();

    public:
        void MainLoop();

    public: // IKeyEventSubscriber
        void onKeyEvent(enum KeyPress, enum Keys) override;

    public: // IMouseEventSubscriber,
        void onMouseEvent(enum Mouse, i32 x, i32 y) override;

    public: // IWindowStateSubscriber
        void onWindowStateEvent(enum WindowState, i32 x, i32 y) override;

    };
}