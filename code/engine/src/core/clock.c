#include "core/clock.h"

#include "platform/platform.h"



void clock_update(struct clock *internal_clock)
{
   if (internal_clock->init_time != 0) 
   {
        internal_clock->elapsed = platform_time() - internal_clock->init_time;
    }
}

void clock_start(struct clock *internal_clock)
{
    internal_clock->init_time = platform_time();
    internal_clock->elapsed = 0;

}
void clock_stop(struct clock *internal_clock)
{
    internal_clock->init_time = 0;
}