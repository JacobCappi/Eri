#pragma once
#include "Core.h"

namespace ERI
{
    class ERI_API App
    {
    public:
        App();
        virtual ~App();
    
    public:
        void MainLoop();
    };
}