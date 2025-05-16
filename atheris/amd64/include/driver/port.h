#ifndef VIPEROS_ATHERIS_AMD64_DRIVER_PORT_H
#define VIPEROS_ATHERIS_AMD64_DRIVER_PORT_H 1

namespace atheris
{
    namespace amd64
    {
        namespace driver
        {
            template <typename DataT>
            class Port
            {
            public:
                constexpr Port(unsigned short port)
                    : mPort(port)
                {
                }

                DataT read() const
                {
                    DataT ret;
                    if constexpr (sizeof(DataT) == 1)
                    {
                        asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(mPort));
                    }
                    else if constexpr (sizeof(DataT) == 2)
                    {
                        asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(mPort));
                    }
                }

                void write(DataT v) const
                {
                    if constexpr (sizeof(DataT) == 1)
                    {
                        asm volatile("outb %0, %1" :: "a"(v), "Nd"(mPort));
                    }
                    else if constexpr (sizeof(DataT) == 2)
                    {
                        asm volatile("outw %0, %1" :: "a"(v), "Nd"(mPort));
                    }
                }

            private:
                unsigned short mPort;
            };
        }
    }
}

#endif // VIPEROS_ATHERIS_AMD64_DRIVER_PORT_H