#include <core/logger.h>
#include <core/asserts.h>
#include <core/application.h>

int main(void)
{
    app_configs config;

    config.start_x = 100;
    config.start_y = 100;
    config.start_height = 720;
    config.start_width = 1280;
    config.app_name = "Eri Engine Testbed";

    app_create(&config);
    app_run();

    return 0;
}