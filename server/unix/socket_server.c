#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "check_request.h"

int main(int argc, char** argv)
{
    if ( --argc!=2 || argv[1] == NULL || argv[2] == NULL)
    {
        puts("Wrong number of arguments");
        puts("Usage: super-happiness-server  path  port");
        exit(1);
    }

    char* path = argv[1]; //init directory
    int sock_fd, clilen, newsock_fd, port = atoi(argv[2]);
    struct sockaddr_in serv_addr, cli_addr;
    char str[INET_ADDRSTRLEN]; //IP client
    char buff[512]; //buffer to receive data

    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    port = 8080;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }


    if (listen(sock_fd,5) < 0)
    {
        perror("ERROR on listening");
        exit(1);
    };

    clilen = sizeof(cli_addr);
    newsock_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsock_fd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN);

    printf("IP CLIENT = %s\n\n",str);

    if ( recv(newsock_fd, buff, 512, 0) < 0 )
    {
        perror("ERROR on recv");
        exit(1);
    };

    isValid(buff, path);


    close(newsock_fd);
    close(sock_fd);
    exit(0);
}