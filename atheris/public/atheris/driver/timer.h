#ifndef VIPEROS_ATHERIS_PUBLIC_DRIVER_TIMER_H
#define VIPEROS_ATHERIS_PUBLIC_DRIVER_TIMER_H 1

/*
 * Generic function to initialize the timer
 * with a specified tick size in nanoseconds
 *
 * Must be provided in atheris/private/driver/timer.h
 *
 * void atheris::timer::Init(int tickSizeNs)
 */

/*
 * 
 */

/*
 * Generic function to subscribe to the timer
 * (note: this removes the previous handler -
 * only one function can be subscribed to
 * the timer at once)
 *
 * Must be provided in atheris/private/driver/timer.h
 *
 * void atheris::timer::Subscribe(void(*handler)())
 */

#include <atheris/private/driver/timer.h>

#endif // VIPEROS_ATHERIS_PUBLIC_DRIVER_TIMER_H