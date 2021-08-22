#include <core/logger.h>
#include <core/asserts.h>

int main(){
    ERI_LOG_FATAL("Testing %s %s", "testing", "working");
    ERI_LOG_ERROR("Testing %s %s", "testing", "working");
    ERI_LOG_WARNING("Testing %s %s", "testing", "working");
    ERI_LOG_INFO("Testing %s %s", "testing", "working");
    ERI_LOG_DEBUG("Testing %s %s", "testing", "working");
    ERI_LOG_TRACE("Testing %s %s", "testing", "working");

    ERI_ASSERT_MESSAGE( 10 == 1, "Testing a message");
    ERI_ASSERT_MESSAGE( 10 == 1, "Testing another message");
    
}