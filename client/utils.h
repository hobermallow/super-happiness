/*
 * utils.h
 *
 *  Created on: Aug 12, 2016
 *      Author: mishima
 */

#ifndef CLIENT_UTILS_H_
#define CLIENT_UTILS_H_

#define MUTEX 1111
#define CLIENTS 2222
#define INTERLOCKED 3333
#define READERS 4444

#include <stdbool.h>

//defining structure for path managing
typedef struct path {
	//string representing a directory
	char* path;
	//boolean targeting recursive behaviour
	bool recusive;

} path_t;


//defining structure for configuration file managing
typedef struct conf_file {
	//pointer to pointer to path_t structures
	path_t** paths;
	//number of paths
	int num_paths;
} conf_file_t;

#endif /* CLIENT_UTILS_H_ */
