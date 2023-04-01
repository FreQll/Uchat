#include "../inc/libmx.h"

int char_count(char * str, char c)
{
	int i = 0, count = 0;
	for (; str[i]; i++)
	{
  		count += (str[i] == c);
	}
	return count;
}

