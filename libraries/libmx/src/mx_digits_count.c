#include "../inc/libmx.h"

int mx_digits_count(unsigned long long n)
{
    if (n == 0) return 1;
    int count = 0;
    while (n != 0) {

        n = n / 10;
        ++count;
    }
    return count;
}

