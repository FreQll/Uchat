#include "../inc/header.h"

int connect_to_server(t_send_param *param) 
{
    struct sockaddr_in serverAddr;
    param->socket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(param->server_port));
    serverAddr.sin_addr.s_addr = inet_addr(param->server_IP);
    return connect(param->socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    
}
int ssl_connect(t_send_param *param) {
    param->ssl = SSL_new(param->ctx);      
    SSL_set_fd(param->ssl, param->socket);    
    if ( SSL_connect(param->ssl) == -1 ) { 
        ERR_print_errors_fp(stderr);
        return 1;
    }
    else
    {
        printf("\n\nConnected with %s encryption\n", SSL_get_cipher(param->ssl));
    }
    return 0;
}

SSL_CTX* initCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms(); 
    SSL_load_error_strings();   
    method = TLS_client_method();  
    ctx = SSL_CTX_new(method);   
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void ShowCerts(SSL *ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); 
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       
        X509_free(cert);    
    }
    else
        printf("Info: No client certificates configured.\n");
}

int skip_bytes(t_send_param *param, int num_bytes_to_skip) {
    char buf[1024];
    int bytes_read = 0;
    int total_bytes_read = 0;
    while (total_bytes_read < num_bytes_to_skip) {
        int bytes_to_read = sizeof(buf);
        if (bytes_to_read > num_bytes_to_skip - total_bytes_read) {
            bytes_to_read = num_bytes_to_skip - total_bytes_read;
        }
        bytes_read = SSL_read(param->ssl, buf, bytes_to_read);
        if (bytes_read <= 0) {
            return -1;
        }
        total_bytes_read += bytes_read;
    }
    return 0;
}

int u_recv(t_send_param *param, void* buf, int len) {
    if (param->online_status != CONNECTED) {
        if(param->online_status != NO_LOGIN) {
            u_reconect();
            return -1;
        }
    }
    int actualLen;
    if (SSL_read(param->ssl, &actualLen, sizeof(actualLen)) <= 0) {
        param->online_status = LOST_CONNECT;
        return -1;
    }
    actualLen = ntohl(actualLen);
    if(actualLen > len)
    {
        printf("WARNING: you received not all message\n");
        if (SSL_read(param->ssl, buf, len) <= 0) {
            param->online_status = LOST_CONNECT;
            return -1;
        }
        if (skip_bytes(param, actualLen - len) < 0) {
            param->online_status = LOST_CONNECT;
            return -1;
        }
    }
    else {
        if (SSL_read(param->ssl, buf, actualLen) <= 0) {
            param->online_status = LOST_CONNECT;
            return -1;
        }
    }
    if (SSL_write(param->ssl, OK_CODE, 2) <= 0) {
        param->online_status = LOST_CONNECT;
        return -1;
    }
    return actualLen;
}

int u_send(t_send_param *param, void* buf, int len) {
    if (param->online_status != CONNECTED) {
        if(!(param->online_status == NO_LOGIN && 
           (((char*)buf)[0] == LOGIN || ((char*)buf)[0] == SIGNUP))) {
            u_reconect();
            return -1;
        }
    }
    int len_n = htonl(len);
    if (SSL_write(param->ssl, &len_n, sizeof(len_n)) <= 0) {
        param->online_status = LOST_CONNECT;
        return -1;
    }
    if (SSL_write(param->ssl, buf, len) <= 0) {
        param->online_status = LOST_CONNECT;
        return -1;
    }
    char response_buff[2];
    if (SSL_read(param->ssl, response_buff, 2) <= 0) {
        param->online_status = LOST_CONNECT;
        return -1;
    }
    if (response_buff[0] == OK_CODE[0]) {
        //printf("[INFO] Successfully sent message.\n");
    }
    else if (response_buff[0] == ERROR_CODE[0])
    {
        printf("[ERROR] Got an error in urecv\n");
        return 0;
    }
    return len;
}

void u_reconect() {
    fflush(stdout);
    printf("online status %d\n", param->online_status);
    if (param->online_status == CONNECTED) {
        return;
    }
    if (param->online_status == NO_LOGIN) {
        if(app->username_t != NULL && app->password_t != NULL) {
            char action[] = {LOGIN, '\0'};
            char * server_query = create_query_delim_separated(3, action, app->username_t, app->password_t); // have to store a hash password
            int online = send_server_request(param, server_query);
            printf("AUTO_LOGIN_%d\n", online);
            fflush(stdout);
            free(server_query);
            if (online == 1) {
                clear_chat_list();
                delete_all_history();
                printf("CONNECTED\n");
                param->online_status = CONNECTED;
                return;
            }
            if (online == -1) {
                param->online_status = LOST_CONNECT;
                return;
            }
            printf("ERROR in reconect\n");
        }
        return;
    }
    if (param->online_status == NO_CONNECTED) {
        if (connect_to_server(param) >= 0) {
            if(ssl_connect(param) == 0) {
                param->online_status = NO_LOGIN;
            }
            else {
                param->online_status = LOST_CONNECT;
            }
        }
        return;
    }
    if (param->online_status == LOST_CONNECT) {
        printf("DISCONNECTED\n");
        show_reconnect();
        SSL_shutdown(param->ssl); // закінчення SSL-з'єднання
        SSL_free(param->ssl);
        close(param->socket);
        param->online_status = NO_CONNECTED;
        return;
    }
}

void show_reconnect() {
    gtk_image_set_from_file(GTK_IMAGE(app->chat_icon), RECONNECT_GIF_PATH);
    gtk_label_set_text(GTK_LABEL(app->status), "Reconnecting...");
    gtk_label_set_text(GTK_LABEL(app->chat_label_info), "");
    open_error_window("Lost connection to the server, trying to reconnect...");
}
