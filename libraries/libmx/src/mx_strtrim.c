#include "../inc/libmx.h"

char *mx_strtrim(const char *str)
{
	if (!str || mx_strlen(str) == 0) return NULL;
	int beg = 0;
	int end = mx_strlen(str);
	for (; mx_isspace(str[beg]); beg++);
	for (; mx_isspace(str[end - 1]); end--);
	char *result = mx_strnew(end - beg + 1);
	result = mx_strncpy(result, str + beg, end - beg);
	return result;
}


