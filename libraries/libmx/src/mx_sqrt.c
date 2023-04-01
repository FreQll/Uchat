#include "../inc/libmx.h"

int mx_sqrt(int x)
{
    if (x == 1)
        return 1;
    else if (x > 1)
	for (int i = 0; i <= x*0.5; i++)
		if (i*i == x)
			return i;
    return 0;
}

