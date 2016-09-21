
#ifndef SUPER_HAPPINESS_MASTER_MANAGER_UPDATES_H
#define SUPER_HAPPINESS_MASTER_MANAGER_UPDATES_H
#endif //SUPER_HAPPINESS_MASTER_MANAGER_UPDATES_H

#include "utils_directory.h"


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

void freeUpdate(Update*);
void freeListUpdates(ListUpdates*);
Update* createUpdate(char*, long, char*, double, char*, bool);
bool addUpdate(ListUpdates*, Update*);
void* check_creation( void*);
int check_update(Directory*);

