#include "../inc/header.h"


sqlite3 * db_init()
{
    sqlite3 * db = NULL;
	sqlite3_open(DB_FILENAME, &db);
	
    format_and_execute(db, "CREATE TABLE IF NOT EXISTS %s ( \
										 username TEXT NOT NULL, \
										 password TEXT NOT NULL, \
										 online TEXT DEFAULT 'offline' NOT NULL, \
										 avatar INTEGER DEFAULT 0 NOT NULL)", 1, USERS_TN);

    //creating table with conversations
    format_and_execute(db, "CREATE TABLE IF NOT EXISTS %s(\
                            chat_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
                            chat_name TEXT NOT NULL,\
							chat_avatar TEXT NOT NULL DEFAULT '?',\
                            chat_members TEXT NOT NULL,\
							loaded_by TEXT NOT NULL DEFAULT '',\
							status TEXT NOT NULL DEFAULT '%s')",2, CHATS_TN, ACTIVE_STATUS);

    //creating table with messages
	format_and_execute(db, "CREATE TABLE IF NOT EXISTS %s(\
                            message_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
                            from_username TEXT NOT NULL, \
                            message_text TEXT NOT NULL, \
                            send_datetime INTEGER NOT NULL, \
                            chat_id INTEGER NOT NULL, \
							loaded_by TEXT NOT NULL DEFAULT '',\
                            status TEXT NOT NULL DEFAULT '%s')", 2, MESSAGES_TN, ACTIVE_STATUS);
    return db;
}


t_list * get_db_data_list(sqlite3 * db, char * selection_query, int cols_count)
{
	sqlite3_stmt * stmt;
	sqlite3_prepare_v2(db, (const char *)selection_query, -1, &stmt, NULL);
	int data_type;
	t_list * table = NULL;
	t_list * row = NULL;
	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		for (int i = 0; i < cols_count; i++)
		{
			data_type = sqlite3_column_type(stmt, i);
			if (data_type == SQLITE_INTEGER)
			{
				mx_push_back(&row, mx_itoa(sqlite3_column_int(stmt, i)));
			}
			else if (data_type == SQLITE_TEXT)
			{
				mx_push_back(&row, mx_strdup((const char *)sqlite3_column_text(stmt, i)));
			}
		}
		mx_push_back(&table, row);
		row = NULL;
	}
	sqlite3_finalize(stmt);
	return table;
}

char ** get_db_data_table(sqlite3 * db, char * template, int colums, int rows, int holders_count, ...)
{
	char query_buf[4096];
 	va_list ptr;
  	va_start(ptr, holders_count);
  	vsprintf(query_buf, template, ptr);
	
	char ** table = malloc(((int)DB_ROWS_MAX)*sizeof(void*));
	sqlite3_stmt * stmt;
	sqlite3_prepare_v2(db, (const char *)query_buf, -1, &stmt, NULL);
	int row_indx = 0;
	while (sqlite3_step(stmt) != SQLITE_DONE && row_indx != rows)
	{
		table[row_indx] = mx_strnew(100000);
		for (int i = 0; i < colums; i++)
		{
			if (!sqlite3_column_text(stmt, i))
			{
				table[row_indx + 1] = NULL;
				delete_table(&table);
				sqlite3_finalize(stmt);
  				va_end(ptr);
				return NULL;			
			}
			table[row_indx] = strcat(table[row_indx], (const char *)sqlite3_column_text(stmt, i));
			if (i != colums - 1)
			{
				table[row_indx] = strcat(table[row_indx], QUERY_DELIM);
			}
		}
		row_indx++;
	}
	table[row_indx] = NULL;
	sqlite3_finalize(stmt);
  	va_end(ptr);
	return table;
}

void clear_inner_list(void * ptr)
{
	t_list * list = ptr;
	if(!list) return;
	t_list * temp = list;
	t_list * copy = temp;
	while (temp)
	{
		copy = temp->next;
		free(temp->data);
		free(temp);
		temp = copy;
	}
	list = temp;
}

void delete_table(char *** table)
{
	if (!table || !*table) return;
	for (int i = 0; (*table)[i]; i++)
	{
		free((*table)[i]);	
	}
	free((*table));
	*table = NULL;
}

int format_and_execute (sqlite3 * db, char * template, int holders_count, ...)
{
	char query_buf[10000];
    char * err = NULL;
	int rc;
    va_list ptr;
    va_start(ptr, holders_count);
	vsprintf(query_buf, template, ptr);
	//printf("Query: %s holders: %d\n", query_buf, holders_count);
	rc = sqlite3_exec(db, (const char *)query_buf, NULL, NULL, &err);
    va_end(ptr);
	if (rc != SQLITE_OK)
	{
		printf("[ERROR] %s\n", err);
		free(err);
		return 1;
	}
	return 0;
}

