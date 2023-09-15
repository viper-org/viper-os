#ifndef VIPER_OS_ALGORITHM_H
#define VIPER_OS_ALGORITHM_H 1

namespace vpr
{
    template <class It, class T>
    constexpr It find(It first, It last, const T& value)
    {
        It current = first;
        while(current != last)
        {
            if (*current == value)
            {
                return current;
            }
            ++current;
        }
        return last;
    }

    template <class It, class UnaryPredicate>
    constexpr It find_if(It first, It last, UnaryPredicate p)
    {
        It current = first;
        while(current != last)
        {
            if (p(*current))
            {
                return current;
            }
            ++current;
        }
        return last;
    }
}

#endif // VIPER_OS_ALGORITHM_H