#include "../inc/header.h"

static guint threadID = 0;

t_chat_message * create_chat_message_struct(char * id, GtkWidget * text_widget)
{
    t_chat_message * message = malloc(sizeof(t_chat_message *));
    message->id = mx_strdup(id);
    message->text = text_widget;
    return message;
}

void delete_chat_message_struct_content(void * message)
{
    if (!message) return;
    t_chat_message * message_struct = (t_chat_message *)message;
    free(message_struct->id);
    free(message_struct);
}

void create_message_widget(char * message_query, bool to_end) 
{
    /*
        MESSAGE QUERY FORMAT: m_id@m_sender_username@m_sended_avatar@m_text@m_send_datetime@m_chat_id
        parts[0] - message id
        parts[1] - from_username
        parts[2] - sender`s avatar
        parts[3] - message_text
        parts[4] - send_datetime
        parts[5] - chat_id
    */
    GtkWidget *message, *icon, *username, *text, *datetime;
    GtkBuilder *builder = gtk_builder_new ();
    GError* error = NULL;
    char *window_path = NULL, *icon_path = NULL;
    char *timestr = NULL;
    char *title = NULL;
    char ** parts = mx_strsplit(message_query, QUERY_DELIM[0]);
    //printf("Got a message query: %s\n", message_query);
    //printf("ID of message: %s\n", parts[0]);
    bool is_user = !strcmp(app->username_t, parts[1]);


    t_message *message_struct;
    message_struct = (t_message*)malloc(sizeof(t_message));

    if (is_user)
    {
        if (!gtk_builder_add_from_file (builder, MY_MESSAGE_UI_PATH, &error)) {
            g_critical ("Couldn't load file: %s", window_path);
        }
    }
    else
    {
        if (!gtk_builder_add_from_file (builder, OTHER_MESSAGE_UI_PATH, &error)) {
            g_critical ("Couldn't load file: %s", window_path);
        }
    }

    message = GTK_WIDGET(gtk_builder_get_object (builder, "message_button"));
    if (!message) {
        g_critical("Window widget error!");
    }

    if (!is_user) {
        icon = GTK_WIDGET(gtk_builder_get_object(builder, "message_icon"));
        username = GTK_WIDGET(gtk_builder_get_object(builder, "message_sender"));
        text = GTK_WIDGET(gtk_builder_get_object(builder, "message_text"));
        datetime = GTK_WIDGET(gtk_builder_get_object(builder, "message_time"));

        gtk_widget_set_name(GTK_WIDGET(username), "message_sender");
    }
    else {
        text = GTK_WIDGET(gtk_builder_get_object(builder, "message_text"));
        datetime = GTK_WIDGET(gtk_builder_get_object(builder, "message_time"));
    }
    mx_push_back(&(app->chat_messages_texts), create_chat_message_struct(parts[0],  text));

    time_t current_time = (time_t)atoi(parts[4]);
    struct tm *time_info;
    char time_string[9]; 

    time_info = localtime(&current_time);

    strftime(time_string, sizeof(time_string), "%H:%M", time_info);

    gtk_widget_set_name(GTK_WIDGET(text), "message_text");

    if(!is_user) 
    {
        asprintf(&icon_path, AVATAR_PATH, parts[2]);
        gtk_label_set_text(GTK_LABEL(username), title);
        gtk_image_set_from_file(GTK_IMAGE(icon), icon_path);
    } 

    GtkWidget *message_id = GTK_WIDGET(gtk_builder_get_object(builder, "message_id"));
    gtk_label_set_text(GTK_LABEL(message_id), parts[0]);

    /*
         parts[0] - message id
        parts[1] - from_username
        parts[2] - sender`s avatar
        parts[3] - message_text
        parts[4] - send_datetime
        parts[5] - chat_id
    */

    gtk_label_set_text(GTK_LABEL(text), restore_input(parts[3]));
    gtk_label_set_text(GTK_LABEL(datetime), time_string);

    const char *label_text;
    label_text = gtk_label_get_text(GTK_LABEL(text));

    const char *id_text;
    id_text = gtk_label_get_text(GTK_LABEL(message_id));

    message_struct->id = atoi(id_text);
    message_struct->message_text = gtk_label_get_text(GTK_LABEL(text));
    if (is_user) message_struct->message_label = text;

    if (is_user) g_signal_connect(message, "button-press-event", G_CALLBACK(my_message_menu), message_struct);
    else g_signal_connect(message, "button-press-event", G_CALLBACK(other_message_menu), message_struct);

    
    gtk_widget_set_name(message, parts[0]);
    if(to_end)
    {
        gtk_box_pack_end(GTK_BOX(app->messages_container), message, false, true, 0);
    }
    else
    {
        gtk_box_pack_start(GTK_BOX(app->messages_container), message, false, true, 0);
    }

    scroll();
    g_object_unref(builder);
    mx_del_strarr(&parts);
}

void create_chat_widget(char * chat_info_query) 
{
    GtkWidget *chat, *icon, *title, *status, *id;
    GtkBuilder *builder = gtk_builder_new ();
    GError* error = NULL;
    char *icon_path = NULL;
    char responce_buff[1000];
    //printf("Got chat info query: %s\n", chat_info_query);
    char ** parts = mx_strsplit(chat_info_query, QUERY_DELIM[0]);

    if (!gtk_builder_add_from_file (builder, CHAT_LIST_UI_PATH, &error)) {
        g_critical ("Couldn't load file: message_other.ui");
    }

    chat = GTK_WIDGET (gtk_builder_get_object (builder, "chat"));
    if (!chat) {
        g_critical ("Window widget error");
    }

    icon = GTK_WIDGET(gtk_builder_get_object(builder, "chat_icon"));
    title = GTK_WIDGET(gtk_builder_get_object(builder, "chat_title"));
    status = GTK_WIDGET(gtk_builder_get_object(builder, "chat_status"));
    id = GTK_WIDGET(gtk_builder_get_object(builder, "chat_id"));

    char query_buff[1000];
    sprintf(query_buff, "%s:%s", parts[3], parts[4]);
    //chat_id@chat_name@chat_avatar@LM_from_username@LM_message_text@LM_message_status@chat_members
    /*
        parts[0] - chat_id
        parts[1] - chat_name
        parts[2] - chat_avatar
        parts[3] - LM_from_username
        parts[4] - LM_message_text
        parts[5] - LM_message_status
        parts + 6 - chat members
    */

    char * another_user = NULL;
    if (!strcmp(parts[1], "?"))
    {
        if (!strcmp(app->username_t, parts[6]))
        {
            gtk_label_set_text(GTK_LABEL(title), parts[7]);   
            another_user = parts[7];
        }     
        else
        {
            gtk_label_set_text(GTK_LABEL(title), parts[6]);
            another_user = parts[6];
        }
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(title), parts[1]);
    }
    gtk_label_set_text(GTK_LABEL(status), query_buff);
    
    asprintf(&icon_path, AVATAR_PATH, parts[2]);
    gtk_image_set_from_file(GTK_IMAGE(icon), icon_path);

    gtk_widget_set_name(chat, parts[0]);

    gtk_list_box_insert(GTK_LIST_BOX(app->chat_list), chat, -1);
        
    g_object_unref(builder);
}

t_chat_message * find_by_message_id(t_list * list, char * m_id)
{
    t_chat_message * message = NULL;
	for (t_list*temp = list; temp; temp = temp->next)
    {
        message = (t_chat_message*)temp->data;
        if (!strcmp(message->id, m_id)) 
        {
            return message;
        }
    }
	return NULL;
}


void process_message_info(char * message_info)
{
    //m_status@m_id@m_sender_username@m_text@m_send_datetime@m_chat_id
    //printf("Got message info: %s\n", message_info);
    char text_buf[1000];
    char ** parts = mx_strsplit(message_info, QUERY_DELIM[0]);
    /*
        parts[0] - message status
        parts[1] - message id
        parts[2] - sender username
        parts[3] - sender avatar
        parts[4] - message text
        parts[5] - send datetime
        parts[6] - chat_id
    */
    GtkWidget * deleted_message_widget;
    t_chat_message * message = NULL;


    if (parts[0][0] == ACTIVE_STATUS[0])
    {
        create_message_widget(message_info + 2, 0);
    }
    else if (parts[0][0] == DELETED_STATUS[0])
    {
        printf("want to delete message widget with id(%s)\n", parts[1]);
        deleted_message_widget = get_message_by_id(parts[1]); 
        gtk_widget_hide(GTK_WIDGET(deleted_message_widget));
    }
    else if (parts[0][0] == EDITED_STATUS[0])
    {
        message = find_by_message_id(app->chat_messages_texts, parts[1]);
        if (message)
        {
            printf("Edit message: ID=(%s), current_text=(%s), change to (%s)\n", message->id, gtk_label_get_text(GTK_LABEL(message->text)), parts[4]);
            gtk_label_set_text(GTK_LABEL(message->text), parts[4]);
        }
        else
        {
            create_message_widget(message_info + 2, 0);    
            printf("No such message\n");
        }
    }
    mx_del_strarr(&parts);
}


void process_chat_info(char * chat_info)
{
    //chat_status@chat_id@chat_name@LM_sender@LM_text@LM_status@chat_members
    char text_buf[10];
    char * chat_id = NULL;
    char c_status = *(tokenize(chat_info, QUERY_DELIM[0], text_buf, 1));
    if (c_status == ACTIVE_STATUS[0])
    {
        create_chat_widget(chat_info + 2);
    }
    else if (c_status == DELETED_STATUS[0])
    {
        chat_id = tokenize(chat_info, QUERY_DELIM[0], text_buf, 2);
        printf("Want to delete a chat with id %s\n", chat_id);
        gtk_widget_hide(GTK_WIDGET(get_chat_by_id(chat_id)));
        if (strcmp(app->current_chat_id, chat_id) == 0) change_chat_by_id(START_PAGE);
    }
}

void find_user() {
    const char *username = gtk_entry_get_text(GTK_ENTRY(app->find_user_entry));
    char request_buff[4096];
    char responce_buff[4096];
    char chat_query_buff[4096];

    char ** info_parts = NULL;
    sprintf(request_buff, "%c%s%s%s%s%s%s", CREATE_CHAT, QUERY_DELIM, "?", QUERY_DELIM, "?", QUERY_DELIM, username);
    printf("server query: %s\n", request_buff);
        
    if(u_send(param, request_buff, strlen(request_buff) + 1) <= 0) return; // send a request to server

    if (u_recv(param, responce_buff, MESSAGE_MAX_LEN) > 0)
    {
        if (responce_buff[0] == OK_CODE[0])
        {
            //process_chat_info(responce_buff + 2);
            printf("[INFO] Chat successfuly created(%s)\n", username);
        }
        else if (responce_buff[0] == RECORD_EXISTS_CODE[0])
        {
            //printf("Chat id to transfter to: %s\n", responce_buff + 2);
            change_chat_by_id(responce_buff + 2);
        }
        else if (responce_buff[0] == ERROR_CODE[0])
        {
            open_error_window(responce_buff + 2);
        }
    }
    mx_printstr(username);
}

void change_chat(GtkListBox* self, GtkListBoxRow* row, gpointer data) 
{
    change_chat_by_id(gtk_widget_get_name(GTK_WIDGET(row)));
}


void change_chat_by_id(const char * new_chat_id) 
{
    if (!new_chat_id || param->online_status != 0) return;
    char query_buff[1000]; 
    // free(app->current_chat_id);
    // app->current_chat_id = mx_strdup(new_chat_id);
    // mx_clear_list(&(app->chat_messages_texts), NULL);
    // app->chat_messages_texts = NULL;

    if (new_chat_id[0] != '0') 
    {
        char *icon_path = NULL;
        gtk_widget_hide(app->welcome_message);
        gtk_widget_show(app->chat_entry_box);
        gtk_widget_show(app->chat_label_info);

        gtk_widget_show(app->chat_icon);
        gtk_widget_show(GTK_WIDGET(app->status));
        sprintf(query_buff, "%s%s%s", RESET_MESSAGES_STATUS, QUERY_DELIM, new_chat_id);
        if(/**/u_send(param, query_buff, strlen(query_buff) + 1) > 0) {
            free(app->current_chat_id);
            app->current_chat_id = mx_strdup(new_chat_id);
            mx_clear_list(&(app->chat_messages_texts), NULL);
            app->chat_messages_texts = NULL;
            if (threadID != 0)
            {
                g_source_remove(threadID);
            }
            delete_all_history();
            threadID = g_timeout_add(200, (GSourceFunc)collect_user_info, (gpointer)"T");
        }
    }
    else 
    {
        gtk_widget_show(app->welcome_message);
        gtk_widget_hide(app->chat_entry_box);
        gtk_widget_hide(app->chat_label_info);
        gtk_widget_hide(app->chat_icon);
        gtk_widget_hide(GTK_WIDGET(app->status));
        delete_all_history();
    }
    scroll();
}

void delete_all_history() {
    GtkContainer *box_container = GTK_CONTAINER(app->messages_container); 
    GList *children, *iter;
    children = gtk_container_get_children(box_container);
    for(iter = children; iter != NULL; iter = iter->next) 
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

void clear_chat_list() 
{
    GtkContainer *box_container = GTK_CONTAINER(app->chat_list); 
    GList *children, *iter;

    children = gtk_container_get_children(box_container);
    for(iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

void send_message() 
{
    if (app->edit_message) return;
    const char *message = gtk_entry_get_text(GTK_ENTRY(app->chat_entry));
    //S@TEXT@TIME@CONVERSATION_ID - send message
    //m_status@m_id@m_sender_username@m_text@m_send_datetime@m_chat_id
    printf("Got a message from input fiend: %s\n", message);
    char action[2] = {SEND_MESSAGE, '\0'};
    char * cur_time = mx_itoa((time(NULL)));
    char * server_query = create_query_delim_separated(4, action, message, cur_time, app->current_chat_id);
    char * message_query = NULL;
    char responce_buff[5100];
    
    printf("Created server query: %s\n", server_query);
    
    if(/**/u_send(param, server_query, strlen(server_query) + 1) > 0) {
        gtk_entry_set_text(GTK_ENTRY(app->chat_entry), "");
        scroll();
    }
    free(server_query);
    free(cur_time);
}

