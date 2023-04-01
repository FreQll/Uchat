#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <poll.h>
#include <sys/types.h>
#include <resolv.h>
#include <errno.h>
#include <sys/stat.h>

#include "queries.h"
#include "../../libraries/openssl/openssl/ssl.h"
#include "../../libraries/openssl/openssl/err.h"
#include "../../libraries/Sqlite3/sqlite3.h"
#include "../../libraries/libmx/inc/libmx.h"
//#include <poll.h>

#define LOAD_MESSAGES_COUNT 20
#define DB_ROWS_MAX 10000
#define MESSAGE_MAX_LEN 10000
#define KEY_LENGHT 4096

#define QUERY_DELIM "@"

//GET REQUESTS
#define GET_CHATS 'V'
#define GET_MESSAGES 'T'
#define GET_COLLOCUTOR_INFO 'K'
#define GET_USER_INFO 'H'

//POST REQUESTS
#define SEND_MESSAGE 'S'
#define CREATE_CHAT 'C'
#define EDIT_MESSAGE 'B'
#define DELETE_MESSAGE 'D'
#define DELETE_CHAT 'E'
#define EXIT_APP 'Q'
#define RESET_MESSAGES_STATUS 'G'
#define UPDATE_USER_INFO 'A'

//STATUS CODES
#define ACTIVE_STATUS "A"
#define EDITED_STATUS "E"
#define DELETED_STATUS "D"

//OTHERS
#define LOGIN 'L'
#define SIGNUP 'R'

//CODES
#define WAIT_FOR_CODE "W"
#define OK_CODE "Y"
#define ERROR_CODE "N"
#define NO_DATA_CODE "E"
#define RECORD_EXISTS_CODE "J"

//DATABASE TABLES NAMES
#define USERS_TN "users"
#define MESSAGES_TN "messages"
#define CHATS_TN "chats"
#define DB_FILENAME "server/database.db"
#define CETRIFS_FILENAME "server/mycert.pem"

typedef unsigned char * code;
#define HOLDERS(a) count_placeholders(a, '%')

typedef struct s_thread_param
{
	sqlite3 *db;
    int socket;
    SSL_CTX *ctx;
    SSL *ssl;
    pthread_mutex_t *mutex_R;
} t_thread_param;

extern int cmdEXIT;
extern int count_of_threads;

void daemonize_process();

//DATABASE UTILS
sqlite3 * db_init();
int count_placeholders(char * str, char c);
void clear_inner_list(void * ptr);
void delete_table(char *** table);
char * mx_strstr_front(const char *haystack, const char *needle);
t_list * get_db_data_list(sqlite3 * db, char * selection_query, int cols_count);

//CRYPTOGRAPHY
code sha256_string(char *string);

//SERVER DATABASE RELATIONSHIP UTILS
char ** get_db_data_table(sqlite3 * db, char * template, int colums, int rows, int holders_count, ...);
char * create_query_delim_separated(int count, ...);
int validate_query(char * code, int delims_count, char * err_message);
int format_and_execute (sqlite3 * db, char * template, int holders_count, ...);

//SERVER UTILS
void* client_thread(void* vparam);
void* exit_thread(void* vparam);
char *encode_login(char *code, t_thread_param *param, bool *online);
void encode(char * code, t_thread_param *param, bool *online, char *userID);

//CONNECTION
int openListener(int port);
SSL_CTX* InitServerCTX(void);
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);
void ShowCerts(SSL* ssl);
int u_recv(t_thread_param *param, void* buf, int len);
int u_send(t_thread_param *param, void* buf, int len);

//NETWORK INTERRATICTION
bool s_to_c_info_exchange(t_thread_param *param, char ** table);

//ECODE FUNCTIONS(REQUESTS PROCESSING)
bool db_login(t_thread_param * param, char ** parts, char ** user);
bool db_signup(t_thread_param * param, char ** parts);
void leave_chat(t_thread_param * param, char * chat_id);
bool update_data_status(t_thread_param * param, char ** table, char * user, char action);
bool user_exists(t_thread_param * param, char * user);
bool create_new_message_record(t_thread_param * param, char * username, char ** parts);
bool create_db_chat_record(t_thread_param *param, char * chat_name, char * chat_avatar, char * members);
bool create_new_chat_record(t_thread_param * param, char * user, char * members_str, char ** parts);
bool get_collocutor_info(t_thread_param * param, char * chat_id, char * user);
bool get_user_info(t_thread_param * param, char * username);
#endif

