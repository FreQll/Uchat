#include "../inc/header.h"

bool s_to_c_info_exchange(t_thread_param *param, char ** table)
{
	if (!table || !*table)
	{
		return 1;
	}
	char query_buff[MESSAGE_MAX_LEN];
	bool online = true;
	int count_of_chats = mx_null_arr_len((char **)table);

	sprintf(query_buff, "%s%s%d", WAIT_FOR_CODE, QUERY_DELIM, count_of_chats);
	u_send(param, query_buff, strlen(query_buff) + 1);
	memset(query_buff, '\0', strlen(query_buff));

	for (int i = 0; table[i]; i++) 
	{
		memset(query_buff, '\0', strlen(query_buff));
		if (online == true) 
		{
			sprintf(query_buff, "%s%s%s", OK_CODE, QUERY_DELIM, (char *)table[i]);
			u_send(param, query_buff, strlen(query_buff) + 1);
			memset(query_buff, '\0', strlen(query_buff));
		}
	}
    printf("[INFO] Successfuly sent %d packages\n", count_of_chats);
	return online;
}

//login / signup requests processing(code has to contain username and password(hash) fields QUERY_DELIM separated)
char *encode_login(char *code, t_thread_param *param, bool *online)
{
	if (!code || !param->db) return NULL;
	char ** parts = mx_strsplit(code, QUERY_DELIM[0]);
	char *user = NULL;
	switch(parts[0][0])
	{
		case LOGIN: // loging
			if (!validate_query(code, 2, "Loging query is wrong, incorrent delimiter count!\n"))
			{
				*online = db_login(param, parts, &user);
			} 
			else 
			{
				if (u_send(param, ERROR_CODE, 2) <= 0) *online = false;
			}
			break;		
		case SIGNUP:
			if (!validate_query(code, 2, "SignUp query is wrong, incorrent delimiter count!\n"))
			{
				*online = db_signup(param, parts);
			} 
			else 
			{
				if (u_send(param, "N", 2) <= 0) *online = false;
			}
			break;
	}
	mx_del_strarr(&parts);
	return user;
}

void encode(char * code, t_thread_param *param, bool *online, char *user)
{
	if (!code || !param->db) return;
	char ** table = NULL;
	char ** parts = mx_strsplit(code, QUERY_DELIM[0]);
	char * members_str = NULL;
	switch(parts[0][0])
	{
		case CREATE_CHAT: //C@NAME@AVATAR@USERNAME1@USERNAME2@... - create new chat 
			members_str = create_query_delim_separated(2, user, code + strlen(parts[0]) + strlen(parts[1]) + strlen(parts[2]) + 3);
			*online = create_new_chat_record(param, user, members_str, parts);
			free(members_str);
			break;
		case GET_CHATS:
			table = get_db_data_table( param->db, GET_CHATS_QUERY, 6, DB_ROWS_MAX, HOLDERS(GET_CHATS_QUERY),
                                          USERS_TN,
                                          user,
                                          MESSAGES_TN,
                                          MESSAGES_TN,
                                          CHATS_TN,
                                          user,
                                          user);
			*online = update_data_status(param, table, user, parts[0][0]);
			delete_table(&table);
			break;
		case DELETE_CHAT: // E@chat_id - leave a chat with chat id
			if (!validate_query(code, 1, "Wrong delim count\n"))
			{
				leave_chat(param, parts[1]);
			}
			break;
		case SEND_MESSAGE: //S@TEXT@TIME@chat_ID - send message
			if (!validate_query(code, 3, "Wrong delim count\n"))
      		{
				format_and_execute(param->db, CREATE_MESSAGE_QUERY, HOLDERS(CREATE_MESSAGE_QUERY), MESSAGES_TN, user, parts[1], parts[2], parts[3]); 
			}
			break;
		case GET_MESSAGES:
			table = get_db_data_table( param->db, GET_MESSAGES_QUERY, 7, DB_ROWS_MAX,  HOLDERS(GET_MESSAGES_QUERY),	
																						MESSAGES_TN,
																						USERS_TN,
																						parts[1],
																						user);
			*online = update_data_status(param, table, user, parts[0][0]);
			delete_table(&table);
			break;
		case RESET_MESSAGES_STATUS: //G@chat_id
			if (!validate_query(code, 1, "Wrong delim count\n"))
      		{
				format_and_execute(param->db, RESET_CHAT_MESSAGES_LOADING_STATUS, HOLDERS(RESET_CHAT_MESSAGES_LOADING_STATUS), MESSAGES_TN, user, parts[1]);
			}
			break;
		case EDIT_MESSAGE: //B@message_id@new_message_text
			if (!validate_query(code, 2, "Wrong delim count\n"))
			{
				format_and_execute(param->db, EDIT_MESSAGE_QUERY, HOLDERS(EDIT_MESSAGE_QUERY), MESSAGES_TN, parts[2], EDITED_STATUS, parts[1]);
			}
			break;
		case DELETE_MESSAGE: // D@message_id - set status=UNDELETED to record in table 'messages' with given message_id
			if (!validate_query(code, 1, "Wrong delim count\n"))
			{
				format_and_execute(param->db, DELETE_MESSAGE_QUERY, HOLDERS(DELETE_MESSAGE_QUERY), MESSAGES_TN, DELETED_STATUS, parts[1]);
			}
			break;
		case GET_COLLOCUTOR_INFO: //K@chat_id
			if (!validate_query(code, 1, "Wrong delim count\n"))
      		{
				*online = get_collocutor_info(param, parts[1], user);
			}
			else
			{
				if (u_send(param, ERROR_CODE, 2) <= 0) *online = false;
			}
			break;
		case GET_USER_INFO: //H@username
			if (!validate_query(code, 1, "Wrong delim count\n"))
      		{
				*online = get_user_info(param, parts[1]);
			}
			else
			{
				if (u_send(param, ERROR_CODE, 2) <= 0) *online = false;
			}
			break;
		case UPDATE_USER_INFO:
			//A@avatar
			format_and_execute(param->db, UPDATE_USER_INFO_QUERY, HOLDERS(UPDATE_USER_INFO_QUERY), USERS_TN, parts[1], user);
			break;
		case EXIT_APP:
			format_and_execute(param->db, GO_OFFLINE_QUERY, HOLDERS(GO_OFFLINE_QUERY),USERS_TN, user);
			format_and_execute(param->db, RESET_LOADING_STATUS, HOLDERS(RESET_LOADING_STATUS) , CHATS_TN, user, user, ACTIVE_STATUS);
			format_and_execute(param->db, RESET_LOADING_STATUS, HOLDERS(RESET_LOADING_STATUS), MESSAGES_TN, user, user, ACTIVE_STATUS);
			format_and_execute(param->db, CLEANUP_DELETED_MESSAGES_QUERY, HOLDERS(CLEANUP_DELETED_MESSAGES_QUERY), MESSAGES_TN, DELETED_STATUS, user);
			format_and_execute(param->db, CLEANUP_DELETED_CHATS_QUERY, HOLDERS(CLEANUP_DELETED_CHATS_QUERY), CHATS_TN, DELETED_STATUS, user);
			break;
	}
	mx_del_strarr(&parts);
}

void* client_thread(void* vparam) 
{
    t_thread_param *param = (t_thread_param*) vparam;
	count_of_threads++;
    printf("---Start-recving-from-new-client---thread-N-%d---\n", count_of_threads);
    bool online = true;
    char *user = NULL;
    char buffer[MESSAGE_MAX_LEN]; //!!!
    while (user == NULL) 
	{
		printf("WAIT\n");
		if (u_recv(param, buffer, MESSAGE_MAX_LEN) <= 0) {
            online = false;
            break;
        }
        //printf(">>>Got a message from unknown client: %s\n", buffer);
        if(buffer[0] == LOGIN || buffer[0] == SIGNUP) {
			pthread_mutex_lock(param->mutex_R);
            user = encode_login(buffer, param, &online);
			pthread_mutex_unlock(param->mutex_R);
        } else {
            //send that u need to login first
        }
		memset(buffer, '\0', MESSAGE_MAX_LEN);
    }
	memset(buffer, '\0', MESSAGE_MAX_LEN);
    
	pthread_mutex_lock(param->mutex_R);    
	format_and_execute(param->db, GO_ONLINE_QUERY, count_placeholders(GO_ONLINE_QUERY, '%'), USERS_TN, user);
	pthread_mutex_unlock(param->mutex_R);
        
	while (online) 
	{
        if (u_recv(param, buffer, MESSAGE_MAX_LEN) <= 0) 
		{
            online = false;
            //OFLINE
			break;
        }
        //printf(">>>Got a message from known client: %s\n", buffer);
		pthread_mutex_lock(param->mutex_R);
		encode(buffer, param, &online, user);
		pthread_mutex_unlock(param->mutex_R);
		memset(buffer, '\0', MESSAGE_MAX_LEN);
    }
	printf("DISCONNECTING User: %s\n", user);

	pthread_mutex_lock(param->mutex_R);
	buffer[0] = EXIT_APP;
	encode(buffer, param, &online, user);
	pthread_mutex_unlock(param->mutex_R);
	
	free(user);
	SSL_shutdown(param->ssl); // закінчення SSL-з'єднання
    SSL_free(param->ssl);
    close(param->socket);
    free(param);
	count_of_threads--;
    pthread_exit(0);
}

void* exit_thread(void* vparam) {
	t_thread_param *param = (t_thread_param*) vparam;
	count_of_threads++;
	printf("Print \"exit\" to terminal to exit)\n");
	struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI , 0};
    char string[10];
	while(1) 
	{
		if( poll(&mypoll, 1, 10) ) {
        	scanf(" %[^\n]s", string);
        	if (mx_strcmp_ic(string, "exit")==0) {
            	cmdEXIT = 1;
				break;
        	}
    	}
	}
    free(param);
	count_of_threads--;
    pthread_exit(0);
}



