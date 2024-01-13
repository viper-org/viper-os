#ifndef VIPEROS_ATHERIS_DRIVER_TIMER_H
#define VIPEROS_ATHERIS_DRIVER_TIMER_H 1

/*
 * Generic function to initialize
 * core-local timer
 *
 * Must be provided in atheris/private/driver/timer.h
 *
 * void atheris::timer::Init()
 */

/*
 * Generic function to register a
 * new handler for the timer on
 * the current core
 *
 * Must be provided in atheris/private/driver/timer.h
 *
 * void atheris::timer::Subscribe(void(*)())
 */

#include <atheris/private/driver/timer.h>

#endif // VIPEROS_ATHERIS_DRIVER_TIMER_H