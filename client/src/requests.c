#include "../inc/header.h"
/*
    S@TEXT@CONVERSATION_ID - send message
	C@NAME@USERNAME1@USERNAME2@... - create new chat 
	F@USERNAME@CONVERSATION_ID - renew chat
	B@MESSAGE_ID
	D@MESSAGE_ID
	E@USERNAME@CONVERSATION_ID
*/

char * create_query_delim_separated(int count, ...)
{
	char * query = mx_strnew(100000);
    va_list ptr;
    // Initializing argument to the
    // list pointer
    va_start(ptr, count);
    for (int i = 0; i < count; i++)
	{
		query = mx_strcat(query, va_arg(ptr, const char *));
		if (i != count - 1)
		{
			query = mx_strcat(query, QUERY_DELIM);
		}
	}
	va_end(ptr);
	return query;
}


int send_server_request(t_send_param *param, char * query)
{
    if (/**/u_send(param, query, strlen(query) + 1) <= 0) {
        perror(strerror(errno));
        return -1;
    }
    char a[1];
    if (/**/u_recv(param, a, 1) <= 0) {
        return -1;
    }
    switch (a[0])
    {
        case 'Y':
            return 1;
        case 'N':
            return 0;
        case 'C':
            return -2; 
        default:
            return -2;
    }
}
