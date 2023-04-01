#include "../inc/libmx.h"

void mx_swap_char(char *s1, char *s2)
{
    char s1_temp = *s1;
    *s1 = *s2;
    *s2 = s1_temp;
}


