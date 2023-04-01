#ifndef STRUCTS_H
#define STRUCTS_H

#include "header.h"

typedef struct s_send_param
{
	int socket;
    char *server_IP;
    char *server_port;
    SSL_CTX *ctx;
    SSL *ssl;
    int online_status;
    int * cmdEXIT;
} t_send_param;

typedef struct s_message {
    int id;
    char *username;
    const char *message_text;
    GtkWidget *message_label;
} t_message;

typedef struct s_chat_message {
    char * id;
    GtkWidget * text;
} t_chat_message;

typedef struct app_s
{
    GtkLabel *status;
    //login widgets
    GtkWidget *login_window;
    GtkWidget *signup_window;
    GtkWidget *username_entry;
    GtkWidget *password_entry;
    GtkWidget *login_button;
    //signup widgets
    GtkWidget *signup_username_entry;
    GtkWidget *signup_password_entry;
    GtkWidget *signup_confirm_password_entry;
    //error hangling widgets
    GtkWidget *error_window;
    GtkWidget *error_label;
    GtkWidget *error_button;
    //chat widgets
    GtkWidget *chat_window;
    GtkWidget *chats_sidebar;
    GtkWidget *username_label;
    GtkWidget *user_icon;
    GtkWidget *chat_icon;
    GtkWidget *messages_container;
    GtkWidget *chat_label_info;
    GtkWidget *chat_list;
    GtkWidget *chat_box;
    GtkWidget *settings_box;
    GtkWidget *chat_scroller;
    GtkWidget *chat_info;
    GtkWidget *welcome_message;
    GtkWidget *send_message_button;
    GtkWidget *chat_entry_box;
    GtkWidget *chat_entry;
    GtkWidget *find_user_entry;
    GtkWidget *theme_combobox;
    GtkWidget *settings_current_icon;
    //widgets for right mouse click
    GtkWidget *my_options;
    GtkWidget *other_options;
    GtkWidget *chat_options;

    GdkPixbufAnimation* reconnect_GIF; 

    char *username;
    char *password;
    const char *username_t;
    const char *password_t;
    char *user_avatar;
    GtkWidget *active_widget;
    const char *active_message;
    char * current_chat_id; // HARDCODED IN APP_INIT!!!!!!!!!!!!!!!!!!!!!!
    bool edit_message;
    t_list * chat_messages_texts;
}              app_t;

app_t *app_init();
extern app_t *app;
extern t_send_param *param;
extern t_message *message_data;
extern int current_window;

#endif
