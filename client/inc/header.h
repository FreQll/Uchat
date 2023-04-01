#ifndef HEADER_H
#define HEADER_H
//libraries
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <poll.h>
#include <errno.h>
#include <stdlib.h>
#include <gtk/gtk.h>

//ssl
//../../libraries/libs/openssl
#include "../../libraries/openssl/openssl/ssl.h"
#include "../../libraries/openssl/openssl/err.h"
#include "../../libraries/libmx/inc/libmx.h"

//our hard work
#include "structs.h"
#include "definitions.h"
#include "chat_functions.h"

#endif


