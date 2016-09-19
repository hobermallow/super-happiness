//
// Created by aPolo on 18/09/2016.
//

#ifndef SUPER_HAPPINESS_MASTER_UTILS_DIRECTORY_H
#define SUPER_HAPPINESS_MASTER_UTILS_DIRECTORY_H

#endif //SUPER_HAPPINESS_MASTER_UTILS_DIRECTORY_H

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


struct directory{
    char* path;
    long size;
    struct directory** subDir;
    int num_subdir;
    struct file_info** files;
    int num_files;

};


struct file_info{
    char* path;
    long size;
    char* rights;
    double timestamp;
};


struct tree_directory{
    struct directory* root;
};


typedef struct directory Directory;
typedef struct file_info FileInfo;
typedef struct tree_directory TreeDir;


bool setFileInfo(char* path, long size, char* rights, long timestamp);

bool setRoot(TreeDir* tree, Directory* dir)
{
    if(tree == NULL || dir == NULL) return false;
    tree->root=dir;
    return true;
};



bool addSubDir(Directory* parent, Directory* subDir)
{
    if(parent == NULL || subDir == NULL){ return false;}
    parent->num_subdir++;

    parent->subDir = (Directory**)realloc(parent->subDir, sizeof(Directory*)*(parent->num_subdir));

    *(parent->subDir + parent->num_subdir -1 ) = subDir;

    return true;
}

bool addFile(Directory* dir, FileInfo* file)
{
    if(dir == NULL || file == NULL) return false;
    dir->num_files++;

    dir->files = (FileInfo**)realloc(dir->files, sizeof(FileInfo*)*(dir->num_files));

    *(dir->files + dir->num_files -1) = file;

    return true;
}


bool isDirectory(char* path)
{
    struct stat buf;
    if( stat(path,&buf) < 0 )
    {
        return false;
    };

    return ( S_ISDIR(buf.st_mode) );
}

/**
 * return true if the directory exists
 */
bool exist(char* path)
{
    DIR *fd;
    return ( (fd = opendir(path) ) == NULL ) ? false : true;
}



/**
 *
 * @param path of the directory
 * @return the size of the directory, else -1
 */
long sizeDir(char* path)
{
    long size=0;
    struct dirent* in_file;
    DIR* fd = opendir(path);

    if( fd == NULL)
    {
        printf("Directory not found\n");
        return -1;
    }

    while( in_file = readdir(fd) )
    {

        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))
            continue;

        size_t length = strlen(in_file->d_name)+strlen(path)+2;
        char *name = calloc(sizeof(char)*length, sizeof(char));

        struct stat buf;

        strncpy(name, path, strlen(path));
        strncat(name, "/", 1);
        strncat(name, in_file->d_name, strlen(in_file->d_name));

        if( stat(name,&buf) < 0 )
        {
            return 0;
        };

        if( isDirectory(name) ) size+=sizeDir(name);

        size+=buf.st_size;
        free(name);
    }

    closedir(fd);
    return size;
}

void setDirectory(Directory* dir, char* path)
{
    dir->path= (char*)calloc((strlen(path)) ,  sizeof(char));
    dir->path = strncpy(dir->path , path, strlen(path));
    dir->size = sizeDir(path);
    dir->subDir = NULL;
    dir->files = NULL;
    dir->num_files=0;
    dir->num_subdir=0;
};


Directory* initTree(char* path, Directory* root, time_t t_server) {


    struct dirent *in_file;
    DIR *fd;

    fd = opendir(path);
    if (fd == NULL) {
        perror("File not found");
        return NULL;
    }

    setDirectory(root,path);
    while (in_file = readdir(fd)) {

        if (!strcmp(in_file->d_name, "."))
            continue;
        if (!strcmp(in_file->d_name, ".."))
            continue;


        size_t length = strlen(in_file->d_name) + strlen(path) + 2;
        char *name = (char*)calloc(length , sizeof(char));

        strncpy(name, path, strlen(path));
        strncat(name, "/", 1);
        strncat(name, in_file->d_name, strlen(in_file->d_name));


        struct stat buf;

        if (stat(name, &buf) < 0) {
            perror("Error: stat");
            return NULL;
        };

        char *perm = calloc(3,sizeof(char));

        perm[0] = buf.st_mode & S_IEXEC ? 'x' : '-'; //x
        perm[1] = buf.st_mode & S_IREAD ? 'r' : '-'; //r
        perm[2] = buf.st_mode & S_IWRITE ? 'w' : '-'; //w


        if( S_ISDIR(buf.st_mode) && in_file->d_name[0] != '.')
        {
            Directory* d = (Directory*)malloc(sizeof(Directory)*1);

            addSubDir(root,d);

            initTree(name,d, t_server);
        }
        else
        {
            FileInfo* fileInfo = (FileInfo*)malloc(sizeof(FileInfo));
            fileInfo->path = (char*)calloc(length, sizeof(char));
            fileInfo->path = strncpy(fileInfo->path, name, length);
            fileInfo->size = buf.st_size;
            fileInfo->rights = (char*)calloc(3, sizeof(char));
            fileInfo->rights = strncpy(fileInfo->rights, perm, 3);
            fileInfo->timestamp = difftime(time(0), t_server);

            addFile(root, fileInfo);
        }
        
        free(perm);
        free(name);
    }

    closedir(fd);

    return root;
}


void visit(Directory* root, int lvl)
{
    int k;
    for(k=lvl ; k>0; k--)
    {
        printf("   ");
    }
    int j;
    for(j=lvl ; j>0; j--)
    {
        if(j==lvl) printf("*");
        printf("_");
    }

    printf("Dir=%s, sz=%ld, n_subDir=%d\n", root->path, root->size, root->num_subdir);

    int f;
    for(f=0; f<root->num_files; f++)
    {
        int k;
        for(k=lvl ; k>0; k--)
        {
            printf("   ");
        }
        printf("file=%s - size=%ld - rights=%s -time=%f\n", root->files[f]->path, root->files[f]->size,
               root->files[f]->rights, root->files[f]->timestamp);
    }

    int i;
    for( i=0; i<root->num_subdir; i++)
    {
        visit( root->subDir[i], lvl+1);
    }
}











