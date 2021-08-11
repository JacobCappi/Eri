#include <core/logger.h>
#include <core/asserts.h>

int main(){
    ERI_LOG_FATAL("YO %s %s", "testing", "working");
    ERI_LOG_ERROR("YO %s %s", "testing", "working");
    ERI_LOG_WARNING("YO %s %s", "testing", "working");
    ERI_LOG_INFO("YO %s %s", "testing", "working");
    ERI_LOG_DEBUG("YO %s %s", "testing", "working");
    ERI_LOG_TRACE("YO %s %s", "testing", "working");

    ERI_ASSERT( 10 == 1);
    ERI_ASSERT_MESSAGE( 10 == 1, "Testing a message");
    ERI_ASSERT_MESSAGE( 10 == 1, "Testing another message");
    
}