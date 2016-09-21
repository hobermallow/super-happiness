#include <sys/types.h>
#include "utils_directory.c"
/*
int main(int argc, char** argv)
{
	char* path = (char*)malloc(sizeof(argv[1]));
    path = argv[1];
	
	//test if exist directory
	printf("Exists ? %s\n", exist(path) ? "true" : "false");

	//test function is a directory
	printf("Is a Directory ? %s\n", isDirectory(path) ? "true" : "false");

	//test size directory
//	printf("The size is : %ld bytes\n", sizeDir(path));

    //test init tree
    TreeDir* tree = (TreeDir*)malloc(sizeof(TreeDir));
    Directory* root = (Directory*)malloc(sizeof(Directory)*1);
    time_t current_time = time(NULL);
    Directory* r = initTree(path, root, current_time);

    //test visit tree
    visit(r,0);


	exit(0);
}*/