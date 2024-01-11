#ifndef VIPEROS_ATHERIS_CPU_SMP_H
#define VIPEROS_ATHERIS_CPU_SMP_H 1

/*
 * Generic function to start up other cores
 * and perform early initialization
 *
 * Must be provided in atheris/private/cpu/smp.h
 *
 * void atheris::cpu::smp::Init()
 */

/*
 * Generic function to send an interrupt to
 * another core with a specified ID
 *
 * Must be provided in atheris/private/cpu/smp.h
 *
 * void atheris::cpu::smp::SendIPI(int, int, atheris::cpu::smp::IPIDestination)
 */

/*
 * Enum representing destinations for IPI
 *
 * Must be provided in atheris/private/cpu/smp.h
 *
 * enum class atheris::cpu::smp::IPIDestination
 * {
 *     Single,
 *     Self,
 *     BroadcastAll,
 *     BroadcastOthers
 * }
 */

/*
 * Enum representing specific IPI vector
 * meanings
 *
 * Must be provided in atheris/private/cpu/smp.h
 *
 * enum atheris::cpu::smp::IPI::IPI
 * {
 *     Panic
 * }
 */

#include <atheris/private/cpu/smp.h>

#endif // VIPEROS_ATHERIS_CPU_SMP_H