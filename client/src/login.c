#include "../inc/header.h"

void open_login_window(void) {
    gtk_widget_hide(app->signup_window);
    show_login_form();
}

GtkWidget *show_login_form() {
    GtkWidget *window;
    GtkBuilder * ui_builder;
    GError * err = NULL;

    current_window = 0;

    ui_builder = gtk_builder_new();
    if(!gtk_builder_add_from_file(ui_builder, LOGIN_UI_PATH, &err)) {
        g_critical ("Couldn't download the UI file : %s", err->message);
        g_error_free (err);
    }
    window = GTK_WIDGET(gtk_builder_get_object(ui_builder, "login_window"));
    if (!window) {
        g_critical ("Window widget error");
    }
    gtk_widget_set_name(GTK_WIDGET(window), "login");
    load_css_from_file();
    
    gtk_builder_connect_signals(ui_builder, NULL);
    app->login_window = window;
    app->username_entry = GTK_WIDGET(gtk_builder_get_object(ui_builder, "username_entry"));
    app->password_entry = GTK_WIDGET(gtk_builder_get_object(ui_builder, "password_entry"));

    app->login_button = GTK_WIDGET(gtk_builder_get_object(ui_builder, "login_button"));
    gtk_widget_show(window);
 
    g_signal_connect(window, "key_press_event", G_CALLBACK (login_enter_keypress), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return window;
}

GtkWidget *open_signup_window(void) {
    gtk_widget_hide(app->login_window);
    GtkWidget *window;
    GtkBuilder * ui_builder;
    GError * err = NULL;

    current_window = 1;

    ui_builder = gtk_builder_new();
    if(!gtk_builder_add_from_file(ui_builder, SIGNUP_UI_PATH, &err)) {
        g_critical ("Couldn't download the UI file : %s", err->message);
        g_error_free (err);
    }

    window = GTK_WIDGET(gtk_builder_get_object(ui_builder, "signup_window"));
    if (!window) {
        g_critical ("Window widget error");
    }
    gtk_widget_set_name(GTK_WIDGET(window), "signup");

    load_css_from_file();
    
    app->signup_window = window;
    app->signup_username_entry = GTK_WIDGET(gtk_builder_get_object(ui_builder, "username_entry"));
    app->signup_password_entry = GTK_WIDGET(gtk_builder_get_object(ui_builder, "password_entry"));
    app->signup_confirm_password_entry = GTK_WIDGET(gtk_builder_get_object(ui_builder, "confirm_password_entry"));

    gtk_builder_connect_signals(ui_builder, NULL);

    gtk_widget_show(window);

    g_signal_connect(window, "key_press_event", G_CALLBACK (signup_enter_keypress), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return window;
}

void log_in(void) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(app->username_entry));
    const char *password = sha256_string((char *)(gtk_entry_get_text(GTK_ENTRY(app->password_entry))));
    int login_error = check_login_data_for_errors(username, password);

    switch (login_error)
    {
    case 0:
        app->username_t = username;
        app->password_t = password;
        param->online_status = CONNECTED;
        open_main_window();
        break;
    case 1:
        open_error_window("Fields cannot be empty.");
        break;
    case 2:
        open_error_window("Login error: Incorrect symbols in username or password.");
        break;
    case 3:
        open_error_window("Login error: Incorrect password or username.");
        break;
    case 4:
        open_error_window("Login error: Cant connect to the server.");
        break;
    }
}

void create_account(void) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(app->signup_username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(app->signup_password_entry));
    const char *confirm_password = gtk_entry_get_text(GTK_ENTRY(app->signup_confirm_password_entry));

    int signup_error = check_signup_data_for_errors(username, password, confirm_password);

    switch (signup_error)
    {
        case 0:
            // mx_printstr("create account with data: ");
            // mx_printstr(username);
            // mx_printchar('\t');
            // mx_printstr(password);
            // mx_printchar('\n');
            open_login_window();
            break;
        case 1:
            open_error_window("Fields cannot be empty.");
            break;
        case 2:
            open_error_window("Signup error: Incorrect symbols in username or password.");
            break;
        case 3:
            open_error_window("Signup error: Username already exists.");
            break;
        case 4:
            open_error_window("Signup error: The passwords must match.");
            break;
        case 5:
            open_error_window("Signup error: Cannot connect to the server.");
            break;
    }
}
