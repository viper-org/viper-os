#include "event/bus.h"
#include "event/object.h"

#include "sched/proc.h"

#include "mm/kheap.h"

static struct event_bus bus;

struct exit_event_object *create_exit_event(struct thread *t)
{
    struct exit_event_object *event = kheap_alloc(sizeof(struct exit_event_object));
    event->t = t;
    event->obj.waiting = 0;
    event->obj.free_on_ready = event;
    event->next = bus.exit_events;
    
    t->exit_event = event;
    
    bus.exit_events = event;

    return event;
}
