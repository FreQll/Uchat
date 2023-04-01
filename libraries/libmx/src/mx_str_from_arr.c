#include "../inc/libmx.h"

char * mx_str_from_arr(char * params[], char * delimiter, bool head, bool tail)
{
    char * full_str = mx_strnew(1000000);
    if (tail) full_str = mx_strcat(full_str, delimiter);
    for (int i = 0; params[i]; i++)
    {
        if (params[i + 1] || head)
        {
            full_str = mx_strcat(mx_strcat(full_str, params[i]), delimiter);
        }
        else
        {
            full_str = mx_strcat(full_str, params[i]);
        }
    }
    return full_str;
} 


