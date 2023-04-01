#include "../inc/libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *s1_ptr = s1;
    const unsigned char *s2_ptr = s2;
    unsigned int i = 0;
    if (n == 0) return 0;
    else {
        while (s1_ptr[i] == s2_ptr[i] && i != n) {
            if (s1_ptr[i] == '\0' && s2_ptr[i] == '\0')
                return 0;
            i++;
        }
        return s1_ptr[i] - s2_ptr[i];
    }
}



