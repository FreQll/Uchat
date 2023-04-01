#include "../inc/libmx.h"

void mx_str_reverse(char *s)
{
    int r = 0;
    int l = mx_strlen(s) - 1;
    for (; r < l; ) mx_swap_char(&s[r++], &s[l--]);
}


