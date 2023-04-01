#include "../inc/libmx.h"

void *mx_memrchr(const void *s, int c, size_t n)
{
    unsigned char *s_ptr = (unsigned char *)s + mx_strlen(s);
	while (n--) 
    {
		if (*s_ptr == (unsigned char)c)
			return s_ptr;
		s_ptr--;
	}
	return NULL;
}



