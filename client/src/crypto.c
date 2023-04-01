#include "../inc/header.h"

char * sha256_string(char *string)
{
    char * hash = malloc(65*sizeof(char));
    unsigned char out[32];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(out, &sha256);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) 
	{
        sprintf((char *)hash + (i * 2), "%02x", out[i]);
    }
    return hash;
}

