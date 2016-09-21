#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "utils_directory.h"



//checks number of input arguments on server
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


//checks validity command and path of the init directory
bool check_command_path(char *command, char *path_init)
{
    char com[4];
    char path_request[512];


    if ( sscanf(command, "%4s %512s", com, path_request) != 2 )
    {
        perror("ERROR on sscanf");
        exit(1);
    };


    if ( !(strcmp(com,"ADDP")==0 || strcmp(com,"INFO")==0 || strcmp(com,"INNR")==0 || strcmp(com,"DISC")==0 ))
    {
        perror("ERROR on command");
        exit(1);
    }

    char* path_complexive = (char*)calloc((strlen(path_init)+strlen(path_request)), sizeof(char));

    strncat(path_complexive, path_init, strlen(path_init));
    strncat(path_complexive, path_request, strlen(path_request));

    printf("Path complexive = %s\n",path_complexive);
    if (!isDirectory(path_complexive)) return false;

    printf("Command is : %s  -  Path is : %s\n",com, path_complexive);

    memset(com, 0 , 4);
    memset(path_request, 0 , 512) ;

    free(path_complexive);

    return true;
}