#include "../inc/libmx.h"

char*mx_strcat(char *restrict s1, const char *restrict s2)
{
    int len1 = mx_strlen(s1);
    int len2 = mx_strlen(s2);
    s1[len2 + len1 + 1] = '\0';
    for (int i = 0; i < len2 + len1 + 1; i++)
        s1[len1 + i] = s2[i];
    return s1;
}

