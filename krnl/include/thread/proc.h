#ifndef VIPEROS_THREAD_PROC
#define VIPEROS_THREAD_PROC 1

// this worker thread cleans up all processes that have finished, including their address spaces

void kthread_proc_init(void);
void kthread_proc_awake(void);

#endif // VIPEROS_THREAD_PROC
