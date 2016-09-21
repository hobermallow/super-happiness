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

void freeFileInfo(FileInfo*);
void freeDirectory(Directory*);
bool setRoot(TreeDir*, Directory*);
bool addSubDir(Directory*, Directory*);
bool addFile(Directory*, FileInfo*);
bool isDirectory(char*);
bool exist(char*);
long sizeDir(char*);
void setDirectory(Directory*, char*);
FileInfo* setFileInfo(char*, struct stat, char*, time_t);
Directory* initTree(char*, Directory*, time_t);
bool contains(Directory*, char*);
void visit(Directory*, int);
Directory* searchDirectory(Directory*, char*);
