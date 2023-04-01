#include "../inc/libmx.h"

int mx_null_arr_len(char * arr[])
{
    if (!arr || !arr[0]) return 0;
    int i = 0;
    for (; arr[i]; i++);
    return i;
}

