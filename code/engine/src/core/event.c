#include "core/event.h"

#include "core/mem.h"

// Arbitrary number of events. Technically codes is u16, so can expand much higher
#define MAX_EVENT_CODES 1500

typedef struct event_code_subscriber
{
    void *subscriber;
    on_raised_event callback;
} event_code_subscriber;

typedef struct event_code_all_subscribers
{
    event_code_subscriber *subscribers;
} event_code_all_subscribers;

typedef struct event_handler
{
    event_code_all_subscribers all_events[MAX_EVENT_CODES];
} event_handler;