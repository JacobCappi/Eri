#pragma once
#include "Core.h"

namespace ERI
{
    class ERI_API App
    {
    private:
        bool _isRunning;
        const f64 _fps = 30.0;
        const f64 _time_per_frame = (1.0 / _fps) * 1000;

        f64 _frame_time = 0;

    public:
        App();
        virtual ~App();
    
    public:
        void MainLoop();
    };
}