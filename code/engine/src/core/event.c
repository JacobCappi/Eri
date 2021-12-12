
#include "core/event.h"
#include "core/logger.h"
#include "core/mem.h"
#include "data_structs/darray.h"

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


static b8 is_init = FALSE;
static event_handler event_system;

b8 init_event(void)
{
    if (is_init != FALSE)
    {
        ERI_LOG_WARNING("Event system is already init");
        return FALSE;
    }

    eri_memzero(&event_system, sizeof(event_handler));
    is_init = TRUE;

    return TRUE;
}

b8 subscribe_event(u16 event_code, void *subscriber, on_raised_event callback)
{
    if (is_init == FALSE)
    {
        return FALSE;
    }
    else
    {
        if (event_system.all_events[event_code].subscribers == 0)
        {
            event_system.all_events[event_code].subscribers = darray_create_default(event_code_subscriber);
        }
        u64 subscriber_count = darray_get_size(event_system.all_events[event_code].subscribers);

        for (u64 i = 0; i < subscriber_count; i++)
        {
            if (event_system.all_events[event_code].subscribers[i].subscriber == subscriber)
            {
                ERI_LOG_WARNING("Subscriber subscribed to the same event twice");
                return FALSE;
            }
        }
        event_code_subscriber event;
        event.subscriber = subscriber;
        event.callback = callback;

        darray_push(event_system.all_events[event_code].subscribers, event);
        return TRUE;
    }
}

ERI_API b8 unsubscribe_event(u16 event_code, void *subscriber, on_raised_event);
ERI_API b8 raise_event(u16 event_code, void *publisher, event_args data);
