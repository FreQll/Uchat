#include "../inc/libmx.h"


char *mx_strtrim_spec(const char *str, char s)
{
    if (!str || mx_strlen(str) == 0) return NULL;
    int beg = 0;
    int end = mx_strlen(str);
    for (; str[beg] == s; beg++);
    for (; str[end - 1] == s; end--);
    char *result = mx_strnew(end - beg + 1);
    result = mx_strncpy(result, str + beg, end - beg);
    return result;
}

