#include <std/thread/mutex.h>

namespace vpr
{
    void mutex::lock()
    {
        while(locked);
        bool expected = false;
        do locked.compare_exchange(expected, true);
        while (expected);
    }

    bool mutex::try_lock()
    {
        bool expected = false;
        locked.compare_exchange(expected, true);
        return expected;
    }

    void mutex::unlock()
    {
        locked = false;
    }
}