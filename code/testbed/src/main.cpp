#include <core/Logger/Logger.h>
// TODO: Never do this, just for testing
#include <Platform/Platform.h>


int main(void)
{
    ERI::Logger logger;
    ERI::Platform platform(logger);

    platform.init_windowing("Test Eri", 0, 0, 800, 800);

    double x = 10.2;


    logger.log_debug() << "Hello world, x = " << x << "\n";
    logger.log_error() << "Hello world, x = " << x << "\n";
    logger.log_fatal() << "Hello world, x = " << x << "\n";
    logger.log_trace() << "Hello world, x = " << x << "\n";
    logger.log_warn() << "Hello world, x = " << x << "\n";
    logger.log_info() << "Hello world, x = " << x << "\n";
    
    return 0;
}