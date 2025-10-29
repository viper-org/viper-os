#ifndef VIPEROS_EVENT_OBJECT
#define VIPEROS_EVENT_OBJECT 1

struct thread;

struct event_object
{
    void *free_on_ready;
    struct thread *waiting;
};

struct exit_event_object
{
    struct event_object obj;
    struct thread *t;

    struct exit_event_object *next;
};

void wait_on_object(struct event_object *, struct thread *);
void ready_event(struct event_object *);

#endif // VIPEROS_EVENT_OBJECT
