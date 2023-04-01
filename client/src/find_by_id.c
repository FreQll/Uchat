#include "../inc/header.h"

GtkWidget *get_message_by_id(char * id) 
{
    GtkContainer *box_container = GTK_CONTAINER(app->messages_container);
    GList *children, *iter;
    children = gtk_container_get_children(box_container);
    printf("Entering the cycyle\n");
    for(iter = children; iter != NULL; iter = iter->next) 
    {
        const char *name = gtk_widget_get_name(GTK_WIDGET(iter->data));
        printf("Got a name: %s\n", name);
        if (strcmp(id, name) == 0) 
        {
            printf("[INFO] Such widget was found\n");
            return iter->data;
        }
    }
    g_list_free(children);
    return NULL;
}

GtkWidget *get_chat_by_id(char * id) 
{
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(app->chat_list));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        GtkWidget *child = GTK_WIDGET(iter->data);
        const char *child_name = gtk_widget_get_name(child);
        if (child_name != NULL && strcmp(child_name, id) == 0) {
            return child;
        }
    }
    return NULL;
    g_list_free(children);
}
