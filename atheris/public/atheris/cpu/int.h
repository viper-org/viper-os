#ifndef VIPEROS_ATHERIS_PUBLIC_CPU_INT_H
#define VIPEROS_ATHERIS_PUBLIC_CPU_INT_H 1

/*
 * Generic function to "push" the interrupt disable
 * flag
 *
 * Must be provided in atheris/private/cpu/int.h
 *
 * void atheris::cpu::PushDisableInt()
 */

/*
 * Generic function to "pop" the interrupt disable
 * flag. If there have been an equal number of
 * pushes and pops, interrupts are enabled
 *
 * Must be provided in atheris/private/cpu/int.h
 *
 * void atheris::cpu::PopDisableInt()
 */

#include <atheris/private/cpu/int.h>

#endif // VIPEROS_ATHERIS_PUBLIC_CPU_INT_H