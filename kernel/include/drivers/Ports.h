#ifndef VIPER_OS_DRIVERS_PORTS_H
#define VIPER_OS_DRIVERS_PORTS_H 1

template<class T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

template<class T, class U>
struct is_same : integral_constant<bool, false> {};

template<class T>
struct is_same<T, T> : integral_constant<bool, true> {};

template<typename DataT>
class Port
{
public:
    Port(unsigned short port)
        :m_Port(port)
    {
    }


    DataT read()
    {
        DataT result;
        if(is_same<DataT, unsigned char>::value)
            asm volatile("inb %1, %0" : "=a"(result) : "Nd"(m_Port));
        else if(is_same<DataT, unsigned short>::value)
            asm volatile("inw %1, %0" : "=a"(result) : "Nd"(m_Port));
        return result;
    }

    void write(DataT data)
    {
        if(is_same<DataT, unsigned char>::value)
            asm volatile("outb %0, %1" :: "a"(data),  "Nd"(m_Port));
        else if(is_same<DataT, unsigned short>::value)
            asm volatile("outw %0, %1" :: "a"(data),  "Nd"(m_Port));
    }
private:
    unsigned short m_Port;
};

#endif // VIPER_OS_DRIVERS_PORTS_H