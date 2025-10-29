#ifndef VIPEROS_EVENT_OBJECT
#define VIPEROS_EVENT_OBJECT 1

struct thread;

struct event_object
{
    struct thread *waiting;
};

void wait_on_object(struct event_object *, struct thread *);
void ready_event(struct event_object *);

#endif // VIPEROS_EVENT_OBJECT
