#ifndef VIPEROS_LIBC_CTYPE
#define VIPEROS_LIBC_CTYPE 1

#ifdef __cplusplus
extern "C" {
#endif


int isalnum(int ch);
int isalpha(int ch);
int islower(int ch);
int isupper(int ch);
int isdigit(int ch);
int isxdigit(int ch);
int iscntrl(int ch);
int isgraph(int ch);
int isspace(int ch);
int isblank(int ch);
int isprint(int ch);
int ispunct(int ch);

int tolower(int ch);
int toupper(int ch);


#ifdef __cplusplus
}
#endif

#endif // VIPEROS_LIBC_CTYPE
