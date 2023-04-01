#include "../inc/libmx.h"

char * mx_itoa(int number){
    char * num = malloc(20 * sizeof(char));
    int i = 0;
    int n = number;
	while (1){
    	if (n % 10 < 0){
    	 	num[i++] = 48 +-(n % 10);
		}
		else{
			num[i++] = 48 + n % 10;
		}
        n /= 10;
		if (n == 0) break;
	}
	if (number < 0){
		num[i++] = '-';
	}
	num[i] = '\0';
	mx_str_reverse(num);
    return num;
}
    

