#include <cpu/Context.h>

#include <string.h>

namespace cpu
{
    void SaveContext(Context* dest, Context* source)
    {
        memcpy(dest, source, sizeof(Context));
    }
}