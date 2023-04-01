#include "../inc/libmx.h"

int mx_count_words(const char *str, char delimiter)
{
    int num = 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] != delimiter)
        {
                for (int j = i + 1; str[j] != delimiter; j++)
                {
                    if (str[j] == '\0') break;
                    i = j;
                }
                num++;
        }    
    return num;
}


