/** socket.h
 *
 *  Created on: Aug 10, 2016
 *      Author: and22
 */

#ifndef DEBUG_CLIENT_UNIX_H_
#define DEBUG_CLIENT_UNIX_H_

#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
#include "../utils.h"


/**
 * @return int corresponding to socket
 */
int create_socket() {
	//defining socket fd
	int s;
	printf("Initializing socket... ");
	//create socket
	if((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		//return -1 if socket is not created
		printf("[FAIL]\n");
		puts("Error while creating socket");
		exit(-1);
	}
	puts("[OK]");
	//return value of socket fd
	return s;
}

//converting ip address to struct in_addr suitable format
void convert_ip_to_address(const char* ip, struct sockaddr_in* addr) {
	//converting ip address in dotted notation
	//to a suitable form for struct in_addr
	addr->sin_addr.s_addr = inet_addr(ip);
//	addr->sin_addr.S_un.S_addr =  inet_addr(ip);
	if(addr->sin_addr.s_addr == INADDR_NONE) {
		puts("Error in converting ip address");
		exit(-1);
	}
}

//read file lines, returning a pointer to pointer
conf_file_t* read_lines(FILE* conf_file) {
	//pointer to conf_file_t struct
	conf_file_t* paths;
	paths = malloc(sizeof(conf_file_t*));
	//array index-iterator
	int i, j, byte_read, line_size = 100, num_lines = 100;
	//initializing number of lines (100 lines)
	paths->paths = malloc(sizeof(path_t*)*100);
	//just use array as paths->paths
	//read file line by line
	for(i = 0; ; i++) {
		if(i > num_lines) {
			num_lines += 100;
			//realloc of array size
			realloc(paths->paths, sizeof(path_t*)*num_lines);
		}
		//repristinating line_size
		line_size = 100;
		//initializing path_t structure for corresponding line
		paths->paths[i] = malloc(sizeof(path_t));
		//initializing path variable inside the structure
		paths->paths[i]->path = malloc(sizeof(char)*line_size);
		//iterating over a line
		for(j = 0; ; j++) {
			//exceeding path buffer
			if(j > line_size) {
				//reallocate line size
				line_size += 100;
				realloc(paths->paths[i]->path, sizeof(char)*line_size);
			}
			//reading char by char
			byte_read = fread(paths->paths[i]->path+j, 1, 1, conf_file);
			//recursive path
			if(j == 0 && *(paths->paths[i]->path+j) == '+') {
				//print for debugging
				paths->paths[i]->recusive = true;
			}
			//if new line
			if(*(paths->paths[i]->path+j) == '\n') {
				//substituting end of string to new line
				paths->paths[i]->path[j] = '\0';
				break;
			}
			if(byte_read == 0) {
				//reached end of file
				//saving line counts in configuration file
				paths->num_paths = i+1;
				return paths;
			}

		}
	}
	//function should exit when reading no character
	//if it reaches this point, something went wrong
	printf("[FAIL]\n");
	puts("Error while parsing configuration file ");
	exit(-1);

}

//main function, updates monitoring loop
void monitor_updates(conf_file_t* paths, struct sockaddr_in server) {
	//variables

	//value for setsockopt
	int value = 1;
	//bytes read
	int bytes, sender_size;
	//socket
	int sock;
	//receiving buffer
	char buffer[4096];
	//client address info
	struct sockaddr_in client, sender;
	//mutex
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutex_attr;

	client.sin_addr.s_addr = INADDR_ANY;
	client.sin_port = htons((unsigned int)9090);
	client.sin_family = AF_INET;

	sock = create_socket();
	//preventing error "Address already in use"
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &value, sizeof(value));

	//binding socket to 9090 port and server address
	if(bind(sock,(struct sockaddr*) &client, sizeof(client)) != 0){
		perror("Binding error: ");
		exit(-1);
	}

	//mutex initializing
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&mutex, &mutex_attr);

	shm_open()

	//

	while(1) {


		//receiving bytes and saving onto temporary struct sockaddr_in
		bytes = recvfrom(sock, buffer, 4096, MSG_PEEK, &sender, &sender_size);
		printf("Bytes read: %d\n", bytes);
		if(bytes == -1) {
			perror("");
			exit(-1);
		}
		if(bytes > 0) {
			buffer[4096] = '\0';
			printf("%s\n", buffer);
		}
	}

}

#endif /* DEBUG_CLIENT_UNIX_H_ */
