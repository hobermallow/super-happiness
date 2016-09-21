#include "manager_updates.h"

void freeUpdate(Update* up)
{
    free(up->path);
    free(up->alteration);
    free(up);
}


void freeListUpdates(ListUpdates* lst)
{
    int i;
    for(i=0; i<lst->num_update; i++)
    {
        freeUpdate(lst->list[i]);
    }
    free(lst->list);
    free(lst);
}


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

void* check_creation( void* root)
{
    Directory* dir = (Directory *)root;
    struct dirent *in_file;
    DIR *fd;

    fd = opendir(dir->path);
    if (fd == NULL)
    {
        perror("File not found");
        return NULL;
    }


    printf("I'm the fucking thread\n I'm going to check the path = %s\n",dir->path);


    while (in_file = readdir(fd)) {

        if (!strcmp(in_file->d_name, "."))continue;
        if (!strcmp(in_file->d_name, "..")) continue;

        size_t length = strlen(in_file->d_name) + strlen(dir->path) + 2;
        char *name = (char *) calloc(length, sizeof(char));

        strncpy(name, dir->path, strlen(dir->path));
        strncat(name, "/", 1);
        strncat(name, in_file->d_name, strlen(in_file->d_name));

        if ( contains(dir, name) == false)
        {
            printf("ATTENTION   New file = %s\n", name);
        }


        free(name);
    }



    return 0;
}



int check_update(Directory* root)
{
    if( ! isDirectory(root->path))
    {

    } else
    {
        check_creation(root);
    }


    return 0;
}