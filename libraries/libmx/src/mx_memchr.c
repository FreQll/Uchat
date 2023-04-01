#include "../inc/libmx.h"

void * mx_memchr(const void *s, int c, size_t n)
{
    if (s)
    {
        const unsigned char * s_ptr = s;
        for (size_t i = 0; s_ptr[i] != '\0'; i++)
            if (s_ptr[i] == (unsigned char)c)
                return (void *)(s_ptr + i);
            else if (i == n)
                break;
    }         
    return NULL;
}

