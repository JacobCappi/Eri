#include "App.h"

// TODO: Temp
#include "Subsystems/Logger/BasicLogger/BasicLogger.h"

#include "Subsystems/EventSystem/VectorEvents/VectorEvents.h"
#include "Subsystems/EventSystem/Events/EventKeyPress.h"

namespace ERI
{

void test(int x, int y)
{
    return;
}
App::App()
{
}
App::~App()
{
}

void App::MainLoop()
{
    // TODO: remove, here just for debugging
    auto log = new BasicLogger();
    log->SetLogLevel(false);

    log->LogInfo("Hello World");
    log->LogDebug("Hello World");
    log->LogError("Hello World");
    log->LogWarning("Hello World");
    log->LogTrace("Hello World");

    auto events = new VectorEvents();
    events->RegisterLogger(log);
    events->Startup();

    u64 key = events->SubscribeKeyPress(Keys::A, test);
    events->PublishKeyPress(Keys::A, 1, 1);

    events->UnsubscribeKeyPress(Keys::A, key);
    bool test = events->PublishKeyPress(Keys::A, 1, 1);

    log->LogDebug("%d", test);
}
    
} // namespace ERI
