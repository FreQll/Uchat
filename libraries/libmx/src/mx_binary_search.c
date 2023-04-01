#include "../inc/libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
	if (!arr) return -1;
	int lb = 0;
	int rb = size - 1;
	int cur = (lb + rb) / 2;
	if (arr)
		while (lb <= rb)
		{
			(*count)++;
			if (mx_strcmp(s, arr[cur]) > 0) lb = cur + 1;	 
			else if (mx_strcmp(s, arr[cur]) < 0) rb = cur - 1;	 
			else return cur;
			cur = (lb + rb) / 2;
		}	
	*count = 0;
	return -1;
}


