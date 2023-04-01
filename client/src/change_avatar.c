#include "../inc/header.h"

void change_user_avatar(char * avatar_id)
{
    char *path = NULL;
    asprintf(&path, AVATAR_PATH, avatar_id);
    free(app->user_avatar);
    app->user_avatar = mx_strdup(avatar_id);
    gtk_image_set_from_file(GTK_IMAGE(app->user_icon), path);
    gtk_image_set_from_file(GTK_IMAGE(app->settings_current_icon), path);
}

void change_avatar(GtkWidget *widget, gpointer data) 
{
    char query_buff[1000];
    const gchar *id = gtk_widget_get_name(widget);
    sprintf(query_buff, "%s%s%s", UPDATE_USER_INFO, QUERY_DELIM, id);
	/**/u_send(param, query_buff, strlen(query_buff) + 1);
}


