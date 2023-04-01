#include "../inc/header.h"

bool db_login(t_thread_param * param, char ** parts, char ** user)
{
	char * db_query = "SELECT password FROM %s WHERE username='%s%s'";
	char ** table = get_db_data_table(param->db, db_query, 1, DB_ROWS_MAX, HOLDERS(db_query), USERS_TN, QUERY_DELIM, parts[1]);
	bool online = true;
	if (!table || !(*table)) // NO SUCH USERNAME IN DATABASE
	{
		if (u_send(param, ERROR_CODE, 2) <= 0) online = false;
		online = false;
	}
	else
	{
		if (!strcmp(table[0], parts[2]))
		{
			*user = mx_strjoin(QUERY_DELIM, parts[1]);
			online = true;
			if (u_send(param, OK_CODE, 2) <= 0) online = false;
		}
		else
		{
			u_send(param, ERROR_CODE, 2);
			online = false;
		}
	}
	delete_table(&table);
	return online;
}


bool db_signup(t_thread_param * param, char ** parts)
{
	bool online = true;
	char * db_query = "SELECT password FROM %s WHERE username='%s%s'";
	char ** table = get_db_data_table(param->db, db_query, 1, DB_ROWS_MAX, HOLDERS(db_query),USERS_TN, QUERY_DELIM, parts[1]);

	if (!table || !(*table))
	{
		db_query = "INSERT INTO %s(username, password) VALUES('%s%s', '%s')";
		format_and_execute(param->db, db_query, count_placeholders(db_query, '%'), USERS_TN, QUERY_DELIM, parts[1], parts[2]);
		online = true;
		if (u_send(param, OK_CODE, 2) <= 0) online = false;
	}
	else
	{
		u_send(param, ERROR_CODE, 2); 
		online = false;
	}
	delete_table(&table);
	return online;
}

void leave_chat(t_thread_param * param, char * chat_id)
{
	char * db_query = "SELECT chat_members FROM %s WHERE chat_id=%s";
	char ** table = get_db_data_table(param->db, db_query, 1, DB_ROWS_MAX, HOLDERS(db_query), CHATS_TN, chat_id);
	if (!table || !*table)
	{
		printf("[ERROR] Data not found\n");
	}
	else
	{
		db_query = "UPDATE %s SET loaded_by='', status='%s' WHERE chat_id=%s";
		format_and_execute(param->db, db_query, 3, CHATS_TN, DELETED_STATUS, chat_id);
		format_and_execute(param->db, db_query, 3, MESSAGES_TN, DELETED_STATUS, chat_id);
	}
	delete_table(&table);
}

bool update_data_status(t_thread_param * param, char ** table, char * user, char action)
{
  bool online = s_to_c_info_exchange(param, table);
  if (!online)
  {
	return false;
  }
  char error_buf[1000];
  char text_buf[1000];
  if (table && *table)
  {
    for (int i = 0; table[i]; i++)
    {
      if (tokenize(table[i], QUERY_DELIM[0], text_buf, 2)) // message or chat id
      {
		printf("action: %s\n", text_buf);
        switch(action)
        {
			case GET_CHATS:
				format_and_execute(param->db, UPDATE_CHAT_STATUS_QUERY, HOLDERS(UPDATE_CHAT_STATUS_QUERY),CHATS_TN, user, text_buf);
				break;
			case GET_MESSAGES:
				format_and_execute(param->db, UPDATE_MESSAGES_STATUS_QUERY, HOLDERS(UPDATE_MESSAGES_STATUS_QUERY), MESSAGES_TN, user, DELETED_STATUS, CHATS_TN, ACTIVE_STATUS, text_buf);
				break;
        }
        memset(text_buf, '\0', strlen(text_buf));
      }
    }
  }
  else
  {
    sprintf(error_buf, "%s%sNo data", NO_DATA_CODE, QUERY_DELIM);
    if (u_send(param, error_buf, strlen(error_buf) + 1) <= 0) online = false;
  }
  return online;
}

bool user_exists(t_thread_param * param, char * user)
{
	char * db_query = "SELECT 1 FROM %s WHERE username='%s%s'";
	char ** table = get_db_data_table(param->db, db_query, 1, 1, HOLDERS(db_query), USERS_TN, QUERY_DELIM, user);
	if (table && *table)
	{
		delete_table(&table);
		return true;	
	}
	delete_table(&table);
	return false;
}

bool create_db_chat_record(t_thread_param *param, char * chat_name, char * chat_avatar, char * members)
{
	int newchat_id = 1;
	char * db_query = "SELECT max(chat_id) FROM %s";
	char ** table = get_db_data_table(param->db, db_query, 1, 1, HOLDERS(db_query), CHATS_TN);
	bool online = true;
	if (table && *table)
	{
		newchat_id += atoi(table[0]) + 1;
	}
	
	if (format_and_execute(param->db, CREATE_CHAT_QUERY, HOLDERS(CREATE_CHAT_QUERY), CHATS_TN, newchat_id, chat_name, chat_avatar, members, ACTIVE_STATUS) != 0)
	{
		if (u_send(param, ERROR_CODE, 2) <= 0) online = false;
	}
	else
	{
		if (u_send(param, OK_CODE, 2) <= 0) online = false;
	}
	
	delete_table(&table);
	return online;
}

bool create_new_chat_record(t_thread_param * param, char * user, char * members_str, char ** parts)
{
	int members_int = mx_null_arr_len(parts + 3) + 1;
	char * another_user = NULL;
	char error_buf[1000];
	char query_buff[1000];
	char * db_query = NULL;
	char ** table = NULL;
	bool online = true;
	/*//C@NAME@AVATAR@USERNAME1@USERNAME2@... - create new chat 
	*/
	printf("members str: %s\n", members_str);
	if (char_count(members_str, QUERY_DELIM[0]) == members_int)
	{
		if (members_int == 2) // dialog
		{
			if (!strcmp(user, parts[3])) another_user = parts[4];    
			else another_user = parts[3];

			if (!user_exists(param, another_user))
			{
				sprintf(error_buf, "%s%sNo such user", ERROR_CODE, QUERY_DELIM);
				if (u_send(param, error_buf, strlen(error_buf) + 1) <= 0) online = false;
				return online;
			}
			else
			{
				db_query = "SELECT chat_id FROM %s WHERE chat_members LIKE '%%%s%%' AND chat_members LIKE '%%%s%%'  AND status!='%s'";
				table = get_db_data_table(param->db, db_query, 1, 1, HOLDERS(db_query), CHATS_TN, user, another_user, DELETED_STATUS);
				if (table && *table) // chat exists
				{
					sprintf(query_buff, "%s%s%s", RECORD_EXISTS_CODE, QUERY_DELIM, table[0]);
					if (u_send(param, query_buff, strlen(query_buff) + 1) <= 0) online = false;
					delete_table(&table);
					return online;
				}
				delete_table(&table);
			}
		}
		online = create_db_chat_record(param, parts[1], parts[2], members_str);
	}
	else
	{
		sprintf(error_buf, "%s%sFailed to create a chat", ERROR_CODE, QUERY_DELIM);
		if (u_send(param, error_buf, strlen(error_buf) + 1) <= 0) online = false;
	}
	return online;
}

bool get_collocutor_info(t_thread_param * param, char * chat_id, char * user)
{
	bool online = true;
	char buff[1000];
	
	char * db_query = "SELECT username, online, avatar \
							FROM %s u \
							INNER JOIN %s c \
							WHERE c.chat_id=%s AND REPLACE(c.chat_members, '%s', '')=username";
	char ** table = get_db_data_table(param->db, db_query, 3, 1, HOLDERS(db_query), USERS_TN, CHATS_TN, chat_id, user);
	if (table && *table)
	{
		sprintf(buff, "%s%s%s", OK_CODE, QUERY_DELIM, table[0]);
		if (u_send(param, buff, strlen(buff) + 1) <= 0) online = false;
	}
	else
	{
		sprintf(buff, "%s%sNo data", NO_DATA_CODE, QUERY_DELIM);
		if (u_send(param, buff, strlen(buff) + 1) <= 0) online = false;
	}
	delete_table(&table);
	return online;
}

bool get_user_info(t_thread_param * param, char * username)
{
	bool online = true;
	char query_buff[1000];
	char ** table = get_db_data_table( param->db, GET_USER_INFO_QUERY, 1, 1, HOLDERS(GET_USER_INFO_QUERY), USERS_TN, QUERY_DELIM, username);
	sprintf(query_buff, "%s%s%s", OK_CODE, QUERY_DELIM, table[0]);
	if (u_send(param, query_buff, strlen(query_buff) + 1) <= 0) online = false;
	delete_table(&table);
	return online;
}

