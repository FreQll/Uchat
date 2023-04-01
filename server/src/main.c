#include "../inc/header.h"

int cmdEXIT;
int count_of_threads;

int main(int argc, char ** argv) 
{
    if (argc != 2) 
    {
		mx_printerr("usage: ./userver [port]\n");
        exit(EXIT_FAILURE);
	}
    //SQL DATABASE TABLES INITIALISATION
    
    daemonize_process();
    
    //OPEN LOGGING FILE AND REDIRECT OUTPUT TO THERE
    int out = open("server/cout.log", O_RDWR | O_CREAT | O_APPEND, 0600);
    if (-1 == out) { perror("cout.log"); return 255; }

    // int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    // if (-1 == err) { perror("cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(out, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }
    
    //OPEN LOGGING FILE AND REDIRECT OUTPUT TO THERE
    sqlite3 * db = db_init();
    
    SSL_CTX *ctx;
    // Initialize the SSL library
    SSL_library_init();
    ctx = InitServerCTX();        /* initialize SSL */
    LoadCertificates(ctx, CETRIFS_FILENAME, CETRIFS_FILENAME);
    int welcomeSocket = openListener(atoi(argv[1]));
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(welcomeSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    pthread_mutex_t mutex_R;
    pthread_mutex_init(&mutex_R, NULL);
    cmdEXIT = 0;
    count_of_threads = 0;
    //creating of exit_thread to correct exit
    pthread_t e_thread;
    t_thread_param* param = (t_thread_param*) malloc(sizeof(t_thread_param));
    int status = pthread_create(&e_thread, NULL, exit_thread, param);
    if (status != 0) 
    {
        printf("main error: can't create thread");
        exit(1);
    }

    while (cmdEXIT == 0) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);

        int client = -1;
        while (cmdEXIT == 0) {
            struct timeval tv;
            tv.tv_sec = 1; // таймаут у секундах
            tv.tv_usec = 0;

            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(welcomeSocket, &rfds);

            int ret = select(welcomeSocket + 1, &rfds, NULL, NULL, &tv);
            if (ret == -1) {
                perror("select");
                exit(EXIT_FAILURE);
            } else if (ret == 0) {
                fflush(stdout);
                continue;
            }
            client = accept(welcomeSocket, (struct sockaddr *)&addr, &len);
            if (client == -1) {
                continue;
            }
            break;
        }
        if (client == -1) {
            continue;
        }
        t_thread_param* param = (t_thread_param*) malloc(sizeof(t_thread_param));
        param->ssl = SSL_new(ctx);              // get new SSL state with context 
        SSL_set_fd(param->ssl, client);
        if(SSL_accept(param->ssl) == -1) 
        {
            close(client);
            free(param);
            ERR_print_errors_fp(stderr);
            continue;
        }
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        param->socket = client;
        param->mutex_R = &mutex_R;
        param->db = db;
        pthread_t thread;
        int status = pthread_create(&thread, NULL, client_thread, param);
        if (status != 0) {
           printf("main error: can't create thread");
           
           exit(1);
        }

    }
    while(count_of_threads > 0) {
        //printf("count_of_threads %d\n", count_of_threads);
    }
    pthread_mutex_destroy(&mutex_R);
    sqlite3_close(db);
    close(welcomeSocket);
    SSL_CTX_free(ctx);
    
    // CLOSE LOGGER AND REDIRECT OUTPUT BACK
    fflush(stdout); close(out);
    fflush(stderr); close(out);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);

    return 0;
}

