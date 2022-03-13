#include <core/Logger/Logger.h>


int main(void)
{
    ERI::Logger logger;

    double x = 10.2;

    logger.log_debug() << "Hello world, x = " << x << "\n";
    logger.log_error() << "Hello world, x = " << x << "\n";
    logger.log_fatal() << "Hello world, x = " << x << "\n";
    logger.log_trace() << "Hello world, x = " << x << "\n";
    logger.log_warn() << "Hello world, x = " << x << "\n";
    logger.log_info() << "Hello world, x = " << x << "\n";
    
    return 0;
}