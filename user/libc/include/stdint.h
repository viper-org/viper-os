#ifndef VIPEROS_LIBC_STDINT
#define VIPEROS_LIBC_STDINT 1

typedef char  int8_t;
typedef short int16_t;
typedef int   int32_t;
typedef long  int64_t;

typedef char  int_fast8_t;
typedef short int_fast16_t;
typedef int   int_fast32_t;
typedef long  int_fast64_t;

typedef char  int_least8_t;
typedef short int_least16_t;
typedef int   int_least32_t;
typedef long  int_least64_t;

typedef long intmax_t;
typedef long intptr_t;



typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;

typedef unsigned char  uint_fast8_t;
typedef unsigned short uint_fast16_t;
typedef unsigned int   uint_fast32_t;
typedef unsigned long  uint_fast64_t;

typedef unsigned char  uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int   uint_least32_t;
typedef unsigned long  uint_least64_t;

typedef unsigned long uintmax_t;
typedef unsigned long uintptr_t;



#define INT8_MIN (-0x80)
#define INT16_MIN (-0x8000)
#define INT32_MIN (-0x80000000)
#define INT64_MIN (-0x8000000000000000)

#define INT_FAST8_MIN (-0x80)
#define INT_FAST16_MIN (-0x8000)
#define INT_FAST32_MIN (-0x80000000)
#define INT_FAST64_MIN (-0x8000000000000000)

#define INT_LEAST8_MIN (-0x80)
#define INT_LEAST16_MIN (-0x8000)
#define INT_LEAST32_MIN (-0x80000000)
#define INT_LEAST64_MIN (-0x8000000000000000)

#define INTPTR_MIN (-0x8000000000000000)
#define INTMAX_MIN (-0x8000000000000000)


#define INT8_MAX (0x7F)
#define INT16_MAX (-0x7FFF)
#define INT32_MAX (-0x7FFFFFFF)
#define INT64_MAX (-0x7FFFFFFFFFFFFFFF)

#define INT_FAST8_MAX (-0x7F)
#define INT_FAST16_MAX (-0x7FFF)
#define INT_FAST32_MAX (-0x7FFFFFFF)
#define INT_FAST64_MAX (-0x7FFFFFFFFFFFFFFF)

#define INT_LEAST8_MAX (-0x7F)
#define INT_LEAST16_MAX (-0x7FFF)
#define INT_LEAST32_MAX (-0x7FFFFFFF)
#define INT_LEAST64_MAX (-0x7FFFFFFFFFFFFFFF)

#define INTPTR_MAX (-0x7FFFFFFFFFFFFFFF)
#define INTMAX_MAX (-0x7FFFFFFFFFFFFFFF)


#define UINT8_MAX (0xFF)
#define UINT16_MAX (0xFFFF)
#define UINT32_MAX (0xFFFFFFFF)
#define UINT64_MAX (0xFFFFFFFFFFFFFFFF)

#define UINT_FAST8_MAX (0xFF)
#define UINT_FAST16_MAX (0xFFFF)
#define UINT_FAST32_MAX (0xFFFFFFFF)
#define UINT_FAST64_MAX (0xFFFFFFFFFFFFFFFF)

#define UINT_LEAST8_MAX (0xFF)
#define UINT_LEAST16_MAX (0xFFFF)
#define UINT_LEAST32_MAX (0xFFFFFFFF)
#define UINT_LEAST64_MAX (0xFFFFFFFFFFFFFFFF)

#define UINTPTR_MAX (0xFFFFFFFFFFFFFFFF)
#define UINTMAX_MAX (0xFFFFFFFFFFFFFFFF)



#define INT8_C(x)  ((int_least8_t)x)
#define INT16_C(x) ((int_least16_t)x)
#define INT32_C(x) ((int_least32_t)x)
#define INT64_C(x) ((int_least64_t) x##L)

#define INTMAX_C(x) ((intmax_t) x##L)


#define UINT8_C(x)  ((uint_least8_t)x)
#define UINT16_C(x) ((uint_least16_t)x)
#define UINT32_C(x) ((uint_least32_t)x)
#define UINT64_C(x) ((uint_least64_t) x##UL)

#define UINTMAX_C(x) ((uintmax_t) x##UL)

#endif // VIPEROS_LIBC_STDINT
