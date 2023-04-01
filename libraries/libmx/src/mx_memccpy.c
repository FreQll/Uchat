#include "../inc/libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (((const unsigned char *)src)[i] == (unsigned char)c) {
			((unsigned char *)dst)[i] = ((const unsigned char *)src)[i];
			return (void *)&(((unsigned char *)dst)[i + 1]);
		} else
			((unsigned char *)dst)[i] = ((const unsigned char *)src)[i];
	}
	return NULL;
}

/*int mx_strlen(const char * s);

int main(void)
{
	char str1[] = "This is string.h library function";
	char str2[100];
	char c = 0;
    mx_memccpy(str2, str1, c , mx_strlen(str1));
    printf("His: %s\n", str2);
    memccpy(str2, str1, c , mx_strlen(str1));
	printf("Mine: %s\n", str2);
    return 0;
}*/

