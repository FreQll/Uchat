#include "../inc/header.h"

void collect_user_info(void * info)
{
    if (!info)
    {
        return;
    }
    char * action;
    action = mx_strdup((char *)info);
    char query[1000];

    sprintf(query, "%s%s%s", action, QUERY_DELIM, app->current_chat_id);
    char responce_buff[MESSAGE_MAX_LEN + 100];
    int records_count = 0;
    bool online = true;
    if (/**/u_send(param, query, strlen(query) + 1) <= 0) {
        free(action);
        return;
    }
    if (/**/u_recv(param, responce_buff, MESSAGE_MAX_LEN) <= 0) {
        free(action);
        return;
    }
    printf("Received a buf: %s\n", responce_buff);
    if (responce_buff[0] == WAIT_FOR_CODE[0])
    {
        records_count = atoi(responce_buff + 2);
        memset(responce_buff, '\0', strlen(responce_buff));
        for (int a = 0; a < records_count; a++) 
        {
            if (online) 
            {
                memset(responce_buff, '\0', strlen(responce_buff));
                if (/**/u_recv(param, responce_buff, MESSAGE_MAX_LEN) > 0)
                {
                    if (action[0] == GET_CHATS[0])
                    {
                        process_chat_info(responce_buff + 2);
                    } 
                    else if (action[0] == GET_MESSAGES[0]) 
                    {
                        process_message_info(responce_buff + 2);
                    }
                }
                else
                {
                    online = false;
                }
            }
	    }
        //printf("[INFO] Successfuly received %d packages\n", records_count);
    }
    else if (responce_buff[0] == ERROR_CODE[0])
        printf("[ERROR] %s\n", responce_buff + 2);
    free(action);
}


void apply_collocutor_info()
{
    if (app->current_chat_id[0] == '0') // if chat isn't chosen
    {
        gtk_label_set_text(GTK_LABEL(app->chat_label_info), app->username_t);
        gtk_label_set_text(app->status, "online");
    }
    else
    {
        char query[1000];
        char responce[1000];
        char * icon_path = NULL;
        char ** parts = NULL;
        sprintf(query, "%c%s%s", GET_COLLOCUTOR_INFO, QUERY_DELIM, app->current_chat_id);
        if (/**/u_send(param, query, strlen(query) + 1) <= 0) {
            return;
        }
        if (/**/u_recv(param, responce, MESSAGE_MAX_LEN) <= 0) {
            return;
        }
        //printf("Received a responce: %s\n", responce);
        //Y@username@online@avatar
        if (responce[0] == OK_CODE[0])
        {
            parts = mx_strsplit(responce + 2, QUERY_DELIM[0]);
            gtk_label_set_text(GTK_LABEL(app->chat_label_info), parts[0]);
            gtk_label_set_text(app->status, parts[1]);
            
            asprintf(&icon_path, AVATAR_PATH, parts[2]);
            gtk_image_set_from_file(GTK_IMAGE(app->chat_icon), icon_path);

            mx_del_strarr(&parts);
        }
        else
        {        
            printf("[ERROR] Got something wrong\n");
        }
    }
}

void apply_user_info()
{
    char responce_buff[1000];
    char query_buff[1000];
    sprintf(query_buff, "%s%s%s", GET_USER_INFO, QUERY_DELIM, app->username_t);
    if (/**/u_send(param, query_buff, strlen(query_buff) + 1) <= 0) {
        return;
    }
    if (/**/u_recv(param, responce_buff, MESSAGE_MAX_LEN) <= 0) {
        return;
    }

    if (responce_buff[0] == OK_CODE[0])
    {
        //Y@avatar
        if (strcmp(app->user_avatar, responce_buff + 2)) // do not change icon widget if avatar was changed to same one
        {
            change_user_avatar(responce_buff + 2);
        }
    }
}


GtkWidget *open_main_window(void) 
{
    gtk_widget_hide(app->login_window);
    GtkWidget *window;
    GtkBuilder * ui_builder;
    GError * err = NULL;

    current_window = 2;

    ui_builder = gtk_builder_new();
    if(!gtk_builder_add_from_file(ui_builder, CHAT_UI_PATH, &err)) {
        g_critical ("Couldn't download the UI file : %s", err->message);
        g_error_free (err);
    }

    window = GTK_WIDGET(gtk_builder_get_object(ui_builder, "main_window"));
    if (!window) {
        g_critical ("Window widget error");
    }

    gtk_widget_set_name(GTK_WIDGET(window), "chat");

    app->theme_combobox = GTK_WIDGET(gtk_builder_get_object(ui_builder, "theme_selector"));
    app->chat_window = window;

    load_css_from_file();

    app->user_icon = GTK_WIDGET(gtk_builder_get_object(ui_builder, "user_icon"));
    app->settings_current_icon = GTK_WIDGET(gtk_builder_get_object(ui_builder, "settings_current_icon"));

    app->username_label = GTK_WIDGET(gtk_builder_get_object(ui_builder, "username_label"));
    app->chat_icon = GTK_WIDGET(gtk_builder_get_object(ui_builder, "other_user_icon"));
    app->chat_label_info = GTK_WIDGET(gtk_builder_get_object(ui_builder, "other_user_name"));

    app->chats_sidebar = GTK_WIDGET(gtk_builder_get_object(ui_builder, "chats_sidebar"));
    gtk_widget_set_name(GTK_WIDGET(app->chats_sidebar), "chats_sidebar");

    app->chat_info = GTK_WIDGET(gtk_builder_get_object(ui_builder, "chat_info"));
    gtk_widget_set_name(GTK_WIDGET(app->chat_info), "chat_info");

    GtkWidget *settings_button = GTK_WIDGET(gtk_builder_get_object(ui_builder, "settings_button"));
    gtk_widget_set_name(GTK_WIDGET(settings_button), "settings_button");

    GtkWidget *back_button = GTK_WIDGET(gtk_builder_get_object(ui_builder, "back_button"));
    gtk_widget_set_name(GTK_WIDGET(back_button), "back_button");

    app->chat_list = GTK_WIDGET(gtk_builder_get_object(ui_builder, "chat_list"));
    gtk_widget_set_name(GTK_WIDGET(app->chat_list), "chat_list");

    app->messages_container = GTK_WIDGET(gtk_builder_get_object(ui_builder, "messages_container"));
    gtk_widget_set_name(GTK_WIDGET(app->messages_container), "messages_container");

    app->chat_box = GTK_WIDGET(gtk_builder_get_object(ui_builder, "chat_box"));
    gtk_widget_set_name(GTK_WIDGET(app->chat_box), "chat_box");

    app->settings_box = GTK_WIDGET(gtk_builder_get_object(ui_builder, "settings_box"));
    gtk_widget_set_name(GTK_WIDGET(app->settings_box), "settings_box");

    app->chat_scroller = GTK_WIDGET(gtk_builder_get_object(ui_builder, "chat_scroller"));
    gtk_widget_set_name(GTK_WIDGET(app->chat_scroller), "chat_scroller");

    app->chat_entry = GTK_WIDGET(gtk_builder_get_object(ui_builder, "new_message_entry"));
    app->chat_entry_box = GTK_WIDGET(gtk_builder_get_object(ui_builder, "message_entry"));
    app->find_user_entry = GTK_WIDGET(gtk_builder_get_object(ui_builder, "user_search_bar"));
    app->welcome_message = GTK_WIDGET(gtk_builder_get_object(ui_builder, "welcome_message"));
    gtk_widget_set_name(GTK_WIDGET(app->welcome_message), "welcome_message");
    app->send_message_button = GTK_WIDGET(gtk_builder_get_object(ui_builder, "send_message_button"));
    gtk_widget_set_name(GTK_WIDGET(app->send_message_button), "send_message_button");
    gtk_widget_set_name(GTK_WIDGET(gtk_builder_get_object(ui_builder, "chat_viewport")), "send_message_button");

    app->status = GTK_LABEL(gtk_builder_get_object(ui_builder, "other_user_status"));
    gtk_widget_set_name(GTK_WIDGET(app->status), "status");
    gtk_label_set_text(GTK_LABEL(app->username_label), app->username_t);

    g_timeout_add(500, (GSourceFunc)apply_user_info, NULL); // GET_USER INFO
    g_timeout_add(300, (GSourceFunc)collect_user_info, "V"); // GET_CHATS
    g_timeout_add(500, (GSourceFunc)apply_collocutor_info, NULL);

    app->reconnect_GIF = gdk_pixbuf_animation_new_from_file (RECONNECT_GIF_PATH, NULL);

    gtk_builder_connect_signals(ui_builder, NULL);

    change_chat_by_id(START_PAGE);

    gtk_widget_show(window);

    //signals
    g_signal_connect(window, "key_press_event", G_CALLBACK (chat_enter_keypress), NULL);
    g_signal_connect(window, "key_press_event", G_CALLBACK (enter_escape), NULL);
    g_signal_connect(G_OBJECT(app->chat_list), "row-selected", G_CALLBACK(change_chat), "1");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(app->chat_list), "button-press-event", G_CALLBACK(chat_actions_menu), "1");
    return window;
}
