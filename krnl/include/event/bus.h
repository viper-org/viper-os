#ifndef VIPEROS_EVENT_BUS
#define VIPEROS_EVENT_BUS 1

#include "event/object.h"

struct event_bus
{
    struct exit_event_object *exit_events;
    struct poll_event_object *poll_events;
};

struct exit_event_object *create_exit_event(struct thread *t);
struct poll_event_object *create_poll_event(struct proc_fd *desc);

struct poll_event_object *find_poll_event(struct proc_fd *desc);

struct event_bus *get_event_bus(void);

#endif // VIPEROS_EVENT_BUS
