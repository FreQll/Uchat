#include "../inc/libmx.h"


char * mx_strstr_front(const char *haystack, const char *needle)
{ 
    int size2 = mx_strlen(needle);      
    int size1 = mx_strlen(haystack);
    const char * old_hay = haystack;
    for (int count = 0; count<size1; haystack++, count++) {
        char *temp = mx_strchr(haystack, haystack[0]);
        if(mx_strncmp(temp,needle,size2)==0)
        {
            int res_len = size1 - mx_strlen(haystack) + size2;
            char * result = mx_strncpy(mx_strnew(res_len), old_hay, res_len);
            return result;
        }
    }
    return NULL;
}

