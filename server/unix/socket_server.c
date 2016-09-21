#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "check_request.h"
#include "manager_updates.h"


// argv[1] = init directory
// argv[2] = n. port
// argv[3] = time updating ?!?!?

int main(int argc, char** argv)
{

    /*   ===   CHECK ARGUMENTS    ===   */
    if ( ! check_arguments(argc, argv[1], argv[2] ) ) exit(0);



    char* path_init = argv[1]; //init directory
    int sock_fd, clilen, newsock_fd, port = atoi(argv[2]);


    struct sockaddr_in serv_addr, cli_addr;
    char str[INET_ADDRSTRLEN]; //IP client
    char buff[512]; //buffer to receive data



    /*   ===    INIT DIRECTORIES TREE   ===   */
    Directory *root = (Directory *) malloc(sizeof(Directory));

    if (initTree(path_init, root, time(NULL)) == NULL) {
        perror("ERROR initTree");
        exit(0);
    }
    /* ====================================== */


    /*   ===   START THREAD FOR UPDATING   ===   */
    pthread_t t;
    pthread_create(&t, NULL, (void *) &check_creation, root);
    if( t == 0 )
    {
        perror("ERROR on creating thread");
        exit(1);
    }
    /* ======================================== */



    /*   ===   SOCKET   ===   */
    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd < 0)
    {
        perror("ERROR on opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);


    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }


    if (listen(sock_fd,128) < 0)
    {
        perror("ERROR on listening");
        exit(1);
    };

    clilen = sizeof(cli_addr);


    /*   ===   ACCEPT   ===*/
    while ( newsock_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &clilen) ) {

        inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN);

        printf("IP CLIENT = %s\n\n", str);

        memset(buff, 0 , 512);

        if( recv( newsock_fd, buff, 512, 0 ) < 0 ) {
            perror("ERROR on recv");
            exit(1);
        };

        printf("BUFF = %s\n",buff);

        check_command_path(buff, path_init);

        close(newsock_fd);
    };
    /*   =============================== */

    close(sock_fd);
    exit(0);
}