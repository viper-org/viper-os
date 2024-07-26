#ifndef VIPEROS_ECHIS_STDDEF_H
#define VIPEROS_ECHIS_STDDEF_H 1

#ifdef __cplusplus
extern "C" {
#endif

typedef decltype(sizeof(0)) size_t;
typedef decltype((char*)0-(char*)0) ptrdiff_t;

#define NULL nullptr

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_ECHIS_STDDEF_H