#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils_directory.h"


bool isValid(char* command, char* path)
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

    char* path_complexive = (char*)malloc(sizeof(char) * (strlen(path)+strlen(path_request)));
    strncat(path_complexive, path, strlen(path));
    strncat(path_complexive, path_request, strlen(path_request));

    printf("Command is : %s  -  Path is : %s\n",com, path_complexive);
    printf("Is a directory ? %s\n", isDirectory(path_complexive) ? "true" : "false");

    return true;
}
