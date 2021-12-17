#pragma once

#include "defines.h"

// Forward Declare struct
struct game;

// Wrapper for config needed for window
// see platform layor for specifics
struct app_configs
{
    i16 start_x;
    i16 start_y;
    i16 start_width;
    i16 start_height;
    char* app_name;

};

// ----- Main Loop Functions
ERI_API b8 app_create(struct game* game_instance);
ERI_API b8 app_run(void);
// ----- END