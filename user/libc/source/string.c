#include <string.h>

void *memcpy(void *d, const void *s, size_t size)
{
    char *d1 = d;
    const char *s1 = s;
    while(size--)
    {
        *d1 = *s1;
        ++d1;
        ++s1;
    }
    return d;
}

void *memset(void *d, int ch, size_t size)
{
    unsigned char *p = d;
    while (size--)
    {
        *p = ch;
        ++p;
    }
    return d;
}


int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}

char *strcat(char *d, const char *s)
{
    char *p = d;
    while (*p) ++p;

    while (*s)
        *p++ = *s++;

    *p = 0;
    return d;
}

char *strcpy(char *s1, const char *s2)
{
    char *s = s1;
    while (*s2)
    {
        *s1 = *s2;
        ++s1;
        ++s2;
    }
    *s1 = 0;
    return s;
}

size_t strlen(const char *s)
{
    size_t i = 0;
    while (s[i]) ++i;
    return i;
}


char *strtok(char *str, const char *delim)
{
    static char* buffer;
 
    if (str != NULL)
        buffer = str;
 
    buffer += strspn(buffer, delim);
 
    if (*buffer == '\0')
        return NULL;
 
    char* const tok_begin = buffer;
 
    buffer += strcspn(buffer, delim);
 
    if (*buffer != '\0')
        *buffer++ = '\0';
 
    return tok_begin;
}

char *strtok_s(char *str, const char *delim, char **buffer)
{
    if (str != NULL)
        *buffer = str;
 
    buffer += strspn(*buffer, delim);
 
    if (**buffer == '\0')
        return NULL;
 
    char* const tok_begin = *buffer;
 
    buffer += strcspn(*buffer, delim);
 
    if (**buffer != '\0')
        *(*buffer)++ = '\0';
 
    return tok_begin;
}

size_t strspn(const char *s1, const char *s2)
{
    size_t len = 0;
    if((s1 == NULL) || (s2 == NULL))
        return len;
    while(*s1)
    {
        if(!strchr(s2,*s1))
        {
            return len;
        }
        else
        {
            ++len;
            ++s1;
        }
    }
    return len;
}

size_t strcspn(const char *s1, const char *s2)
{
    size_t len = 0;
    if((s1 == NULL) || (s2 == NULL))
        return len;
    while(*s1)
    {
        if(strchr(s2,*s1))
        {
            return len;
        }
        else
        {
            ++len;
            ++s1;
        }
    }
    return len;
}

char *strchr(const char *str, int ch)
{
    for (int i = 0; str[i]; ++i)
    {
        if (str[i] == ch) return (char *)str + i;
    }
    return NULL;
}
