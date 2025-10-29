#ifndef VIPEROS_EVENT_BUS
#define VIPEROS_EVENT_BUS 1

#include "event/object.h"

struct event_bus
{
    struct exit_event_object *exit_events;
};

struct exit_event_object *create_exit_event(struct thread *t);

#endif // VIPEROS_EVENT_BUS
