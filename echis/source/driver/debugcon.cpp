#include <driver/debugcon.h>

#include <atheris/driver/debugcon.h>

#include <cstdlib>
#include <cstdarg>

namespace echis
{
    namespace driver
    {
        namespace debugcon
        {
            void WriteString(const char* data, std::size_t size)
            {
                for (std::size_t i = 0; i < size; ++i)
                {
                    atheris::driver::debugcon::WriteChar(data[i]);
                }
            }

            void Write(const char* data)
            {
                std::size_t i = 0;
                while (data[i])
                {
                    atheris::driver::debugcon::WriteChar(data[i++]);
                }
            }


            void WriteFormatted(const char* format, ...)
            {
                va_list args;
                va_start(args, format);

                std::size_t i = 0;
                while(format[i])
                {
                    if (format[i] == '%')
                    {
                        ++i;
                        switch (format[i])
                        {
                            case '%':
                                atheris::driver::debugcon::WriteChar('%');
                                break;

                            case 'c':
                            {
                                char c = va_arg(args, int);
                                atheris::driver::debugcon::WriteChar(c);
                                break;
                            }
                            
                            case 's':
                            {
                                char* str = va_arg(args, char*);
                                Write(str);
                                break;
                            }

                            case 'i':
                            case 'd':
                            {
                                int n = va_arg(args, int);
                                char buf[12];
                                Write(std::itoa(n, buf, 10));
                                break;
                            }
                            
                            case 'p':
                            {
                                void* p = va_arg(args, void*);
                                char buf[19] = "0x";
                                Write(std::ptoa(p, buf+2, 16)-2);
                                break;
                            }
                        }
                    }
                    else
                    {
                        atheris::driver::debugcon::WriteChar(format[i]);
                    }
                    ++i;
                }
            }
        }
    }
}