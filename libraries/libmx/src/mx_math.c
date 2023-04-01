#include "../inc/libmx.h"

int mx_ceil(float value)
{
	if (value - (int)value == 0) 
		return value;
	return value + 1;
}

