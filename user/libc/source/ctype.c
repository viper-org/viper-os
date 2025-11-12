#include <ctype.h>

int isalnum(int ch)
{
    if (ch >= 48 && ch <= 57)  return 1;
    if (ch >= 65 && ch <= 90)  return 1;
    if (ch >= 97 && ch <= 122) return 1;
    return 0;
}

int isalpha(int ch)
{
    if (ch >= 65 && ch <= 90)  return 1;
    if (ch >= 97 && ch <= 122) return 1;
    return 0;
}

int islower(int ch)
{
    return ch >= 97 && ch <= 122;
}

int isupper(int ch)
{
    return ch >= 65 && ch <= 90;
}

int isdigit(int ch)
{
    return ch >= 48 && ch <= 57;
}

int isxdigit(int ch)
{
    if (ch >= 48 && ch <= 57)  return 1;
    if (ch >= 65 && ch <= 70)  return 1;
    if (ch >= 97 && ch <= 102) return 1;
    return 0;
}

int iscntrl(int ch)
{
    return ch <= 31 ||  ch == 127;
}

int isgraph(int ch)
{
    return ch >= 33 && ch <= 126;
}

int isspace(int ch)
{
    if (ch >= 9 && ch <= 13) return 1;
    return ch == ' ';
}

int isblank(int ch)
{
    return ch == ' ' || ch == '\t';
}

int isprint(int ch)
{
    return !iscntrl(ch);
}

int ispunct(int ch)
{
    if (ch >= 33 && ch <= 47)   return 1;
    if (ch >= 58 && ch <= 64)   return 1;
    if (ch >= 91 && ch <= 96)   return 1;
    if (ch >= 123 && ch <= 126) return 1;
    return 0;
}


int tolower(int ch)
{
    if (!isupper(ch)) return ch;
    return ch + 32;
}

int toupper(int ch)
{
    if (!islower(ch)) return ch;
    return ch - 32;
}
