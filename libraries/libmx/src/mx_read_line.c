#include "../inc/libmx.h"

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    if (buf_size < 0 || fd < 0)
        return -2;
    (*lineptr) = (char *)mx_realloc(*lineptr, buf_size);
    mx_memset((*lineptr), '\0', malloc_size((*lineptr)));
    size_t bytes = 0;
    char c;
    if (!read(fd, &c, 1)) {
        return -1;
    }
    else{
        if (c == delim) return bytes;
        (*lineptr) = (char *) mx_realloc(*lineptr, bytes + 1);
        (*lineptr)[bytes++] = c;
    }
    for (;read(fd, &c, 1);)
    {
        if (c == delim) break;
        if (bytes >= buf_size)
            (*lineptr) = (char *) mx_realloc(*lineptr, bytes + 1);
        (*lineptr)[bytes] = c;
        bytes++;
    }
    (*lineptr) = (char * )mx_realloc(*lineptr, bytes + 1);
    size_t bytes_left = malloc_size((*lineptr)) - bytes;
    mx_memset(&(*lineptr)[bytes], '\0', bytes_left);
    return bytes;
}

/*int main(void)
{
    char * str = malloc(300*sizeof(char));
    int fd = open("fragment", O_RDONLY);
    int res = mx_read_line(&str, 7, '\n', fd); //res = 8, str = "FADE IN:"
    printf("res=%d, str=%s\n", res, str);
    res = mx_read_line(&str, 35, 'a', fd); //res = 34, str = ""
    printf("res=%d, str=%s\n", res, str);
    res= mx_read_line(&str, 1, '.', fd); //res = 15, str = "s no boundaries"
    printf("res=%d, str=%s\n", res, str);
    res= mx_read_line(&str, 10, '\n', fd); //res = 0, str = ""
    printf("res=%d, str=%s\n", res, str);
    free(str);
    return 0;
}*/


