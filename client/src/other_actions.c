#include "../inc/header.h"

app_t *app_init() 
{
	app_t *app = NULL;
 
	app = (app_t *) malloc(sizeof(app_t));
	if (app == NULL) return NULL;
	memset(app, 0, sizeof(app_t));
	
    app->username = NULL;
    app->password = NULL;
    app->active_message = malloc(1);
    app->current_chat_id = mx_strdup("0");
    app->edit_message = false;
    app->user_avatar = mx_strdup("0");
    app->chat_messages_texts = NULL;
    current_window = 0;
    return app;
}

gboolean chat_enter_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_Return) 
    {
        if (gtk_widget_has_focus(app->chat_entry) && app->edit_message) set_text();
        if (gtk_widget_has_focus(app->chat_entry) && !app->edit_message) send_message();
        if (gtk_widget_has_focus(app->find_user_entry)) find_user();
        return TRUE;
    }
    return FALSE;
}

gboolean login_enter_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_Return) 
    {
        log_in();
        return TRUE;
    }
    return FALSE;
}

gboolean signup_enter_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_Return) 
    {
        create_account();
        return TRUE;
    }
    return FALSE;
}

gboolean enter_escape(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_Escape) {
        change_chat_by_id(START_PAGE);
        return TRUE;
    }
    return FALSE;
}

void scroll() {
    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(app->chat_scroller));
    gtk_adjustment_set_page_size(adjustment, 0);
    double value = gtk_adjustment_get_upper(adjustment);
    gtk_adjustment_set_value(adjustment, value);
    printf("\n SCROLLED WINDOW \n");
}

char * correct_input(char * str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        switch(str[i])
        {
            case '\'':
                str[i] = 0;
                break;
            case '\"':
                str[i] = 1;
                break;
            case '=':
                str[i] = 2;
                break;
            case ';':
                str[i] = 3;
                break;
            case '<':
                str[i] = 4;
                break;
            case '>':
                str[i] = 5;
                break;
        }
    }
    return str;
}

char * restore_input(char * str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        switch(str[i])
        {
            case 0:
                str[i] = '\'';
                break;
            case 1:
                str[i] = '\"';
                break;
            case 2:
                str[i] = '=';
                break;
            case 3:
                str[i] = ';';
                break;
            case 4:
                str[i] = '<';
                break;
            case 5:
                str[i] = '>';
                break;
        }
    }
    return str;
}

void show_settings(void) {
    gtk_widget_hide(app->chat_box);
    gtk_widget_hide(app->chats_sidebar);
    gtk_widget_show(app->settings_box);
}

void back_to_chat(void) {
    gtk_widget_show(app->chat_box);
    gtk_widget_show(app->chats_sidebar);
    gtk_widget_hide(app->settings_box);
}
