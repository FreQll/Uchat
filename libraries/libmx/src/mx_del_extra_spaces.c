#include "../inc/libmx.h"

char *mx_del_extra_spaces(const char *str) {
	if (!str || str[0] == '\0') return NULL;
	char * trimmed = mx_strtrim(str);
	int len = mx_strlen(trimmed);
	int i = 0;
	int j = 0;
	char * result = malloc(len*sizeof(char));
	while (i < len)
	{
		if (!mx_isspace(trimmed[i])){
			result[j++] = trimmed[i++];
		}
		else{
			result[j++] = ' ';
			for (; mx_isspace(trimmed[i]); i++);
		}
	}
	mx_strdel(&trimmed);
	return result;
}



