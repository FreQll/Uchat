#include "../inc/libmx.h"

char * tokenize(char * str, char delim, char * buf, int steps)
{
    if (!str || !buf || char_count(str, delim) < steps) return NULL;
    int delims_count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == delim)
        {
            delims_count++;
            i++;
        }
        if (delims_count == steps - 1)
        {
            int j = 0;
            for (; str[i + j] != delim && str[i + j] != '\0'; j++)
            {
                buf[j] = str[i + j];
            }
            buf[j] = '\0';
            return buf;
        }
    }
    return NULL;
}

