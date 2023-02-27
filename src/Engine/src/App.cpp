#include "App.h"

// TODO: Temp
#include "Logger/BasicLogger/BasicLogger.h"

namespace ERI
{
App::App()
{
}
App::~App()
{
}

void App::MainLoop()
{
    auto log = new BasicLogger();
    log->SetLogLevel(false);

    log->LogInfo("Hello World");
    log->LogDebug("Hello World");
    log->LogError("Hello World");
    log->LogWarning("Hello World");
    log->LogTrace("Hello World");
}
    
} // namespace ERI
