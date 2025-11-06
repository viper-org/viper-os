#include "event/object.h"

#include "driver/dbg.h"
#include "sched/sched.h"

void wait_on_object(struct event_object *obj, struct thread *t)
{
    t->wnext = obj->waiting;
    obj->waiting = t;
}

void ready_event(struct event_object *obj)
{
    dbg_printf("%p\n", obj);
    struct thread *curr = obj->waiting;
    while (curr)
    {
        struct thread *next = curr->wnext;
        sched_readdthread(curr);
        curr = next;
    }
    obj->waiting = NULL;
}
