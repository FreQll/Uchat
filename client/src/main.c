#include "../inc/header.h"

app_t *app;
t_send_param *param;
t_message *message_data;
int current_window;

int main(int argc, char * argv[]) {
    
    if (argc != 3)
    {
        mx_printerr("Usage: ./uclient <server IP> <server port>\n");
        return 0;
    }
    
    int out = open("client/cout.log", O_RDWR | O_CREAT | O_APPEND, 0600);
    if (-1 == out) { perror("cout.log"); return 255; }

    // int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    // if (-1 == err) { perror("cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(out, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }

    app = app_init();
   
    int cmdEXIT = 0;
    //int status_addr;

    message_data = (t_message*)malloc(sizeof(t_message));

    param = malloc(sizeof(t_send_param));
    param->server_IP = argv[1];
    param->server_port = argv[2];
    printf("Input params: %s %s\n", param->server_IP, param->server_port);

    SSL_library_init();
    param->ctx = initCTX();
    
    param->online_status = NO_CONNECTED;
    
    gtk_init(&argc, &argv);
    g_timeout_add(1000, (GSourceFunc)u_reconect, NULL);
    show_login_form();

    gtk_main();
    
    fflush(stdout); close(out);
    fflush(stderr);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    printf("SHUTTED DOWN\n");
    return 0;
}




