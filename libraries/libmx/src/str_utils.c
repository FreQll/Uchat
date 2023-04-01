#include "../inc/libmx.h"

int indexOf(char * str, char c){
	for (int i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == c) return i;
	}
	return -1;
}

void str_repeat(int n, char * s)
{
	for (int i = 0; i < n; i++){
			mx_printstr(s);
	}
}

