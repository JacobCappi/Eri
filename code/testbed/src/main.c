#include <core/logger.h>
#include <core/asserts.h>
#include <platform/platform.h>

int main(){
    ERI_LOG_FATAL("Testing %s %s", "testing", "working");
    ERI_LOG_ERROR("Testing %s %s", "testing", "working");
    ERI_LOG_WARNING("Testing %s %s", "testing", "working");
    ERI_LOG_INFO("Testing %s %s", "testing", "working");
    ERI_LOG_DEBUG("Testing %s %s", "testing", "working");
    ERI_LOG_TRACE("Testing %s %s", "testing", "working");

    platform_state state;
    if(platform_startup(&state, "ERI testbed", 100, 100, 800, 600))
    {
        while(TRUE)
        {
            platform_message(&state);
        }
    }
    platform_shutdown(&state);
}