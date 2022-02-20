#pragma once
#include "defines.h"

struct clock
{
    f64 init_time;
    f64 elapsed;
};

void clock_update(struct clock *internal_clock);
void clock_start(struct clock *internal_clock);
void clock_stop(struct clock *internal_clock);