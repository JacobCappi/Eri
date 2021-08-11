#include <core/logger.h>

int main(){
    LOG_FATAL("YO %s %s", "testing", "working");
    LOG_ERROR("YO %s %s", "testing", "working");
    LOG_WARNING("YO %s %s", "testing", "working");
    LOG_INFO("YO %s %s", "testing", "working");
    LOG_DEBUG("YO %s %s", "testing", "working");
    LOG_TRACE("YO %s %s", "testing", "working");
}