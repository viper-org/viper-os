#include "event/bus.h"
#include "event/object.h"

#include "sched/pipe.h"
#include "sched/proc.h"

#include "mm/kheap.h"

static struct event_bus bus;

struct exit_event_object *create_exit_event(struct thread *t)
{
    struct exit_event_object *event = kheap_alloc(sizeof(struct exit_event_object));
    event->t = t;
    event->obj.waiting = 0;
    event->next = bus.exit_events;
    
    t->exit_event = event;
    
    bus.exit_events = event;

    return event;
}

struct poll_event_object *create_poll_event(struct proc_fd *desc)
{
    struct poll_event_object *event = kheap_alloc(sizeof(struct poll_event_object));
    event->desc = desc;
    event->obj.waiting = 0;
    event->next = bus.poll_events;
    
    desc->pipe->poll_event = event;

    bus.poll_events = event;

    return event;
}
