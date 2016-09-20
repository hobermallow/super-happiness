#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef SUPER_HAPPINESS_MASTER_MANAGER_UPDATES_H
#define SUPER_HAPPINESS_MASTER_MANAGER_UPDATES_H

#endif //SUPER_HAPPINESS_MASTER_MANAGER_UPDATES_H


struct update{
    char* path;
    long size;
    char perm[3];
    double time;
    char* alteration;
    bool isDirectory;
};

struct lst_updates{
    struct update** list;
    long num_update;
};

typedef struct update Update;
typedef struct lst_updates ListUpdates;

Update* createUpdate(char* path, long size, char* perm, double time, char* alteration, bool isDirectory)
{
    if( path==NULL || size==-1 || perm==NULL || alteration==NULL ) return NULL;

    Update* update = (Update*)malloc(sizeof(Update));

    update->path = (char*)malloc(strlen(path) * sizeof(char));
    update->path = strncpy(update->path, path, strlen(path));

    update->size = size;

    update->alteration = (char*)malloc(strlen(alteration) * sizeof(char));
    update->path = strncpy(update->alteration, alteration, strlen(alteration));

    update->isDirectory = isDirectory;
    update->perm[1] = perm[1]; update->perm[2] = perm[2]; update->perm[3] = perm[3];
    update->time = time;

    return update;
}

bool addUpdate(ListUpdates* lst, Update* up)
{
    lst->list = (Update**)realloc(lst->list, lst->num_update * sizeof(Update*));
    if(lst->list == NULL) return false;

    lst->list[lst->num_update] = up;
    return true;
}