#include "../inc/libmx.h"

int mx_get_substr_index(const char *str, const char *sub)
{
    if (!str || !sub) return -2;
    int j;
    for (int i = 0; str[i] != '\0'; i++)
    {
        j = 0;
        while (str[i + j] == sub[j])
        {
            if (sub[j + 1] == '\0')
                return i;
            j++;
        }
    }
    return -1;
}

