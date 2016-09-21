#include <stdio.h>
#include <stdlib.h>
#ifndef SUPER_HAPPINESS_MASTER_MANAGER_CLIENT_H
#define SUPER_HAPPINESS_MASTER_MANAGER_CLIENT_H

#endif //SUPER_HAPPINESS_MASTER_MANAGER_CLIENT_H

struct client{
    char* id;
    char* current_path;
    char** paths;
};

typedef struct client Client;


int openFileManagerClient(char* filename)
{
    int fd;
    return (fd = fopen(filename,"a+b") == NULL) ? -1 : fd;
}


Client* createClient(char* in);

int addPathClient(Client* client, char* path);

int setCurrentPathClient(Client* client, char* path);

int checkPathClient(Client* client, char* path);

