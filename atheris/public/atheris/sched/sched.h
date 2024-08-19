#ifndef VIPEROS_ATHERIS_SCHED_SCHED_H
#define VIPEROS_ATHERIS_SCHED_SCHED_H 1

/*
 * Generic function to prepare a thread
 * to be started
 *
 * Must be provided in atheris/private/sched/user.h
 *
 * void atheris::sched::PrepareThread(echis::sched::Thread*)
 */

/*
 * Opaque structure representing saved
 * context of a thread when switching
 *
 * Must be provided in atheris/private/sched/user.h
 *
 * struct atheris::sched::ThreadContext
 */

/*
 * Generic function to switch thread
 * contexts, saving the old one
 *
 * Must be provided in atheris/private/sched/user.h
 *
 * void atheris::sched::SwitchContext(atheris::sched::ThreadContext**, atheris::sched::ThreadContext*)
 */

#include <atheris/private/sched/sched.h>

#endif // VIPEROS_ATHERIS_SCHED_SCHED_H