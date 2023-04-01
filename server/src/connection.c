#include "../inc/header.h"

// Make current process a daemon
void daemonize_process() 
{

    pid_t pid, sid;
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        printf("PID: %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    umask(0);
    if ((sid = setsid()) < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    signal(SIGTERM, SIG_DFL);

}



// Create the SSL socket and intialize the socket address structure
int openListener(int port)
{
    int sd;
    struct sockaddr_in addr;
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}

SSL_CTX* InitServerCTX(void)
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = TLS_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}

void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
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
    {
        printf("No certificates.\n");
    }
}

int skip_bytes(t_thread_param *param, int num_bytes_to_skip) {
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

int u_recv(t_thread_param *param, void* buf, int len) {
    if (cmdEXIT) {
        return -1;
    }
    int actualLen;
    while (cmdEXIT == 0) {
        struct timeval tv;
        tv.tv_sec = 1; // таймаут у секундах
        tv.tv_usec = 0;

        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(param->socket, &rfds);

        int ret = select(param->socket + 1, &rfds, NULL, NULL, &tv);
        if (ret == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        } else if (ret == 0) {
            fflush(stdout);
            continue;
        }
        break;
    }
    int status = SSL_read(param->ssl, &actualLen, sizeof(actualLen));
    if (status == -1) {
        return -1;
    }
    actualLen = ntohl(actualLen);
    if(actualLen > len) 
    {
        printf("WARNING: you received not all message\n");
        if (SSL_read(param->ssl, buf, len) <= 0) {
            return -1;
        }
        if (skip_bytes(param, actualLen - len) < 0) {
            return -1;
        }
    }
    else 
    {
        if (SSL_read(param->ssl, buf, actualLen) <= 0) 
        {
            return -1;
        }
    }
    if (SSL_write(param->ssl, OK_CODE, 2) <= 0) {
        return -1;
    }
    //printf("[INFO] Successfully recv message.\n");
    return actualLen;
}

int u_send(t_thread_param *param, void* buf, int len) {
    if (cmdEXIT) {
        return -1;
    }
    int len_n = htonl(len);
    if (SSL_write(param->ssl, &len_n, sizeof(len_n)) <= 0) {
        return -1;
    }
    if (SSL_write(param->ssl, buf, len) <= 0) {
        return -1;
    }
    char response_buff[2];
    if (SSL_read(param->ssl, response_buff, 2) <= 0) 
    {
        return -1;
    }
    if (response_buff[0] == OK_CODE[0]) 
    {
        //printf("[INFO] u_send success).\n");
    }
    else if (response_buff[0] == NO_DATA_CODE[0]) 
    {
        printf("[INFO] No data to receive(%s)\n", response_buff);
    }
    else
    {
        printf("[ERROR] usend failure(Undefined package=%s)\n", response_buff);
        return -1;
    }
    return len;
}


