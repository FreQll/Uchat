#ifndef CHAT_FUNCTIONS_H
#define CHAT_FUNCTIONS_H

#include "header.h"

void show_reconnect();

//backend functions
GtkWidget *get_message_by_id(char * id);
GtkWidget *get_chat_by_id(char * id);
void edit_message_by_id(int id, char *text);
t_chat_message * find_by_message_id(t_list * list, char * m_id);
char * sha256_string(char *string);
void delete_chat();
char * correct_input(char * str);
char * restore_input(char * str);

//chat action functions
void log_in(void);
void create_account(void);
void show_settings(void);
void send_message();
void create_message_widget(char * message_query, bool to_end);
void find_user();
gboolean chat_enter_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data);
gboolean login_enter_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data);
gboolean signup_enter_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data);
gboolean enter_escape(GtkWidget *widget, GdkEventKey *event, gpointer data);
void change_chat_by_id(const char * new_chat_id);
void scroll();
void change_chat(GtkListBox* self, GtkListBoxRow* row, gpointer data);
void set_text();
void edit_message(GtkWidget *widget, gpointer data);
void collect_user_info(void * info);
void delete_all_history();
void clear_chat_list() ;
void renew_chat_list();
void apply_collocutor_info();
void change_user_avatar(char * avatar_id);

//functions for right mouse click
gboolean my_message_menu(GtkWidget *widget, GdkEventButton *event, t_message *data);
gboolean other_message_menu(GtkWidget *widget, GdkEventButton *event, t_message *data);
gboolean chat_actions_menu(GtkWidget *widget, GdkEventButton *event, gpointer data);
void create_options_popover(GtkWidget *widget, bool isMy);
void create_chat_options_popover();

//window functions
void open_login_window(void);
GtkWidget *open_signup_window(void);
GtkWidget* show_login_form();
GtkWidget *open_main_window(void);

//error handling
GtkWidget *open_error_window(char *error_message);
bool contains_wrong_char(const char *string);
int check_login_data_for_errors(const char *username, const char *password);
int check_signup_data_for_errors(const char *username, const char *password, const char *c_password);

//css
void load_css(int theme);
void save_user_theme(int theme);
void load_css_from_file();

//working with chats
void create_chat(char * chat_info_query);
void collect_messages(void * info);
void process_message_info(char * message_info);
void process_chat_info(char * chat_info);

//CONECTION
int connect_to_server(t_send_param *param);
int ssl_connect(t_send_param *param);
SSL_CTX* initCTX(void);
void ShowCerts(SSL *ssl);
int u_recv(t_send_param *param, void* buf, int len);
int u_send(t_send_param *param, void* buf, int len);
void u_reconect();

//request utils
char * create_query_delim_separated(int count, ...);
int send_server_request(t_send_param *param, char * query);

#endif
