#ifndef VIPEROS_EVENT_OBJECT
#define VIPEROS_EVENT_OBJECT 1

struct thread;

struct event_object
{
    struct thread *waiting;
};

struct exit_event_object
{
    struct event_object obj;
    struct thread *t;

    int exit_code;

    struct exit_event_object *next;
};

struct poll_event_object
{
    struct event_object obj;
    struct proc_fd *desc;

    struct poll_event_object *next;
};

void wait_on_object(struct event_object *, struct thread *);
void ready_event(struct event_object *);

#endif // VIPEROS_EVENT_OBJECT
