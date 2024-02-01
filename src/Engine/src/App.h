#pragma once
#include "Core.h"

#include "Subsystems/Subsystems.h"

#include "Subsystems/Events/KeyEvents/IKeyEventSubscriber.h"
#include "Subsystems/Events/MouseEvents/IMouseEventSubscriber.h"
#include "Subsystems/Events/WindowEvents/IWindowStateSubscriber.h"

namespace ERI
{
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

    public:
        App();
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