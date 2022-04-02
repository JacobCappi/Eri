#pragma once
#include "defines.h"


namespace ERI
{
    class Application
    {

    private:
        i32 start_x;
        i32 start_y;
        i32 start_width;
        i32 start_height;
        char *app_name;
        Application singleton_app;

        Application();

    public:
        Application get_instance(void);
        ~Application();


    };

}