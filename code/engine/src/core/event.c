
#include "core/event.h"
#include "core/logger.h"
#include "core/mem.h"
#include "data_structs/darray.h"

// Arbitrary number of events. Technically codes is u16, so can expand much higher
#define MAX_EVENT_CODES 1500

struct event_code_subscriber
{
    void *subscriber;
    on_raised_event callback;
};

struct event_code_all_subscribers
{
    struct event_code_subscriber *subscribers;
};

struct event_handler
{
    struct event_code_all_subscribers all_events[MAX_EVENT_CODES];
};


static b8 is_init = FALSE;
static struct event_handler event_system;

b8 init_event(void)
{
    ERI_LOG_INFO("Eri initializing event subsystem...");
    if (is_init == TRUE)
    {
        ERI_LOG_WARNING("Event system is already init");
        return FALSE;
    }

    eri_memzero(&event_system, sizeof(struct event_handler));
    is_init = TRUE;

    return TRUE;
}

void shutdown_event() 
{
    ERI_LOG_INFO("Eri shutting odnw event subsystem");
    // Free the events arrays. And objects pointed to should be destroyed on their own.
    for(u16 i = 0; i < MAX_EVENT_CODES; ++i){
        if(event_system.all_events[i].subscribers != 0) {
            darray_destroy(event_system.all_events[i].subscribers);
            event_system.all_events[i].subscribers = 0;
        }
    }
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
            event_system.all_events[event_code].subscribers = darray_create_default(struct event_code_subscriber);
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
        struct event_code_subscriber event;
        event.subscriber = subscriber;
        event.callback = callback;

        darray_push(event_system.all_events[event_code].subscribers, event);
        return TRUE;
    }
}

b8 unsubscribe_event(u16 event_code, void *subscriber, on_raised_event callback)
{
    if ( is_init == FALSE )
    {
        return FALSE;
    }

    if (event_system.all_events[event_code].subscribers == 0)
    {
        ERI_LOG_WARNING("Subscriber could not unsubscribe. Does not Exists.");
        return FALSE;
    }

    u64 subscriber_count = darray_get_size(event_system.all_events[event_code].subscribers);

    for(u64 i = 0; i < subscriber_count; ++i)
    {
        struct event_code_subscriber e = event_system.all_events[event_code].subscribers[i];
        if(e.subscriber == subscriber && e.callback == callback) 
        {
            struct event_code_subscriber removed_event;
            darray_remove(event_system.all_events[event_code].subscribers, i, &removed_event);
            return TRUE;
        }
    }
    return FALSE;
}

ERI_API b8 raise_event(u16 event_code, void *publisher, struct event_args data)
{
    if ( is_init == FALSE )
    {
        return FALSE;
    }

    if (event_system.all_events[event_code].subscribers == 0)
    {
        ERI_LOG_WARNING("Event ignored, no subscribers.");
        return FALSE;
    }

    u64 subscriber_count = darray_get_size(event_system.all_events[event_code].subscribers);

    for (u64 i; i < subscriber_count; i++)
    {
        struct event_code_subscriber e = event_system.all_events[event_code].subscribers[i];
        if (e.callback(event_code, publisher, e.subscriber, data))
        {
            // event 'handled' can be defined by callback 
            return TRUE;
        }

    }
    return FALSE;
}