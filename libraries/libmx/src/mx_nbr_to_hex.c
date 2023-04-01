#include "../inc/libmx.h"

char *mx_nbr_to_hex(unsigned long nbr)
{
	int count = 0;
	unsigned long temp = nbr;
	for (; temp != 0; count++) temp /= 10;
	char * str_num = mx_strnew(count + 1);
	int i = 0;
	while (nbr != 0) {
        	temp = nbr % 16;
	 	if (temp < 10) str_num[i++] = temp + 48;
		else str_num[i++] = temp + 87;
		nbr = nbr / 16;	
        }
	mx_str_reverse(str_num);
	return str_num;
}



