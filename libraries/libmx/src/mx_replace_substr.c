#include "../inc/libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (!str || !sub || !replace) return NULL;
    int sum = mx_strlen(replace) - mx_strlen(sub);
    int len = mx_strlen(str) + mx_count_substr(str, sub) * sum;
    len += 2;
    char *memory = mx_strnew(len);
    int sub_len = mx_strlen(sub);
    int replace_len = mx_strlen(replace);
    for (int i = 0; i < len; i++, str++) {     
        if (!mx_strncmp((char *)str, (char *)sub, sub_len)) {
            mx_strncpy(&memory[i], replace, replace_len);
            i += replace_len - 1;
            str += sub_len - 1;
        }
        else {
            memory[i] = *str;
        }
    }
    return memory;
}

