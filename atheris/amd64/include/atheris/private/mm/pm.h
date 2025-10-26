// Copyright 2025 solar-mist

#ifndef VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_PM_H
#define VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_PM_H 1

#include <stdint.h>

namespace atheris
{
    namespace pm
    {
        using physaddr_t = uint64_t;

        constexpr int PageSize = 0x1000;
    }
}

#endif // VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_PM_H