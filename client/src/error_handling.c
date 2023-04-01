#include "../inc/header.h"

GtkWidget *open_error_window(char *error_message) {
    GtkWidget *window;
    GtkBuilder * ui_builder;
    GError *err = NULL;

    ui_builder = gtk_builder_new();
    if(!gtk_builder_add_from_file(ui_builder, ERROR_UI_PATH, &err)) {
        g_critical ("Couldn't download the UI file : %s", err->message);
        g_error_free (err);
    }

    window = GTK_WIDGET(gtk_builder_get_object(ui_builder, "error_window"));
    if (!window) {
        g_critical ("Window widget error");
    }
    gtk_widget_set_name(GTK_WIDGET(window), "error");

    load_css_from_file();
    
    app->error_window = GTK_WIDGET(gtk_builder_get_object(ui_builder, "error_window"));
    app->error_label = GTK_WIDGET(gtk_builder_get_object(ui_builder, "error_label"));
    app->error_button = GTK_WIDGET(gtk_builder_get_object(ui_builder, "error_button"));

    switch(current_window) {
        case 0:
            gtk_window_set_transient_for(GTK_WINDOW(app->error_window), GTK_WINDOW(app->login_window));
            break;
        case 1:
            gtk_window_set_transient_for(GTK_WINDOW(app->error_window), GTK_WINDOW(app->signup_window));
            break;
        case 2:
            gtk_window_set_transient_for(GTK_WINDOW(app->error_window), GTK_WINDOW(app->chat_window));
            break;
    }

    gtk_builder_connect_signals(ui_builder, NULL);

    gtk_label_set_text(GTK_LABEL(app->error_label), error_message);

    gtk_widget_show(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return window;
}


int check_login_data_for_errors(const char *username, const char *password) {
    //empty fields
    if (strcmp(username, "\0") == 0 
        || strcmp(password, "\0") == 0) {
        return 1;
    }
    //wrong chars
    if (contains_wrong_char(username) || contains_wrong_char(password)) {
        return 2;
    }
    //if user dont exist
    char action[] = {LOGIN, '\0'};
    char * server_query = create_query_delim_separated(3, action, username, password); // have to store a hash password
    int online = send_server_request(param, server_query);
    free(server_query);
    if (online < 0) {
        return 4;
    }
    if (online == 0) {
        return 3;
    }
    return 0;
}

//we need to take from database only last LOAD_MESSAGES_COUNT ignoring the status value first
//then we need to take LOAD_MESSAGES_COUNT messages with status='unread' only

int check_signup_data_for_errors(const char *username, const char *password, const char *c_password) {
    if (strcmp(username, "\0") == 0 
        || strcmp(password, "\0") == 0
        || strcmp(c_password, "\0") == 0) {
        return 1;
    }
    if (contains_wrong_char(username) || contains_wrong_char(password) || contains_wrong_char(c_password)) {
        return 2;
    }
    if (strcmp(password, c_password) != 0) return 4;
    char action[] = {SIGNUP, '\0'};
    char * server_query = create_query_delim_separated(3, action, username, sha256_string((char *)password)); // have to store a hash password
    
    int online = send_server_request(param, server_query);
    free(server_query);
    if (online < 0) {
        return 5;
    }
    if (online == 0) {
        return 3;
    }
    return 0;
}

bool contains_wrong_char(const char *string) {
    for (int i = 0; i < mx_strlen(string); i++) {
        if (!mx_isalpha(string[i]) && !mx_isdigit(string[i])
            && string[i] != '-' && string[i] != '_') {
            return true;
        }
    }
    return false;
}

void close_error_window() {
    gtk_widget_hide(app->error_window);
}
