#include "../inc/header.h"

int count_placeholders(char * str, char c)
{
	int i = 0, count = 0;
	for (; str[i]; i++)
	{
  		count += (str[i] == c && mx_isalpha(str[i + 1]));
	}
	return count;
}

char * create_query_delim_separated(int count, ...)
{
	char * query = mx_strnew(100000);
    va_list ptr;
    va_start(ptr, count);
    for (int i = 0; i < count; i++)
	{
		query = mx_strcat(query, va_arg(ptr, const char *));
		if (i != count - 1)
		{
			query = mx_strcat(query, QUERY_DELIM);
		}
	}
	va_end(ptr);
	return query;
}

int validate_query(char * code, int delims_count, char * err_message)
{
	if (char_count(code, QUERY_DELIM[0]) != delims_count)
	{
		printf("%s\n", err_message);
		return 1;
	}
	return 0;
}






