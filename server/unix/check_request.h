#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils_directory.h"

bool check_arguments(int argc, char* argv1, char* argv2)
{
    if ( --argc!=2 || argv1 == NULL || argv2 == NULL)
    {
        puts("Wrong number of arguments");
        puts("Usage: super-happiness-server  path_init  port");
        return false;
    }
    return true;
}


bool check_command_path(char *command, char *path_init)
{
    char com[4];
    char path_request[512];

    if ( sscanf(command, "%4s %512s", com, path_request) != 2 )
    {
        perror("ERROR on sscanf");
        exit(1);
    };

    if (!(strcmp(com,"ADDP") || strcmp(com,"INFO") || strcmp(com,"INNR") || strcmp(com,"DISC")))
    {
        perror("ERROR on command");
        exit(1);
    }

    char* path_complexive = (char*)malloc(sizeof(char) * (strlen(path_init)+strlen(path_request)));
    strncat(path_complexive, path_init, strlen(path_init));
    strncat(path_complexive, path_request, strlen(path_request));

    printf("Command is : %s  -  Path is : %s\n",com, path_complexive);
    printf("Is a directory ? %s\n", isDirectory(path_complexive) ? "true" : "false");

    return true;
}
