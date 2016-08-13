/*
 * ms.h
 *
 *  Created on: Aug 10, 2016
 *      Author: and22
 */

#ifndef DEBUG_CLIENT_WINDOWS_MS_H_
#define DEBUG_CLIENT_WINDOWS_MS_H_

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "../utils.h"

SOCKET sock;

SOCKET create_socket() {
	//socket
	SOCKET sock;
	//initializing win socket structure
	WSADATA wsa;
	printf("Initializing windows socket structure... ");
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		puts("Error in Windows socket structure initialization");
		exit(-1);
	}
	puts("[OK]");
	printf("Creating socket... ");
	//create socket
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		puts("Error while creating socket...");
		exit(-1);
	}
	puts("[OK]");
	//socket successfully created
	return sock;
}

//converting ip address to struct in_addr suitable format
void convert_ip_to_address(const char* ip, struct sockaddr_in* addr) {
	//converting ip address in dotted notation
	//to a suitable form for struct in_addr
	addr->sin_addr.S_un.S_addr =  inet_addr(ip);
	if(addr->sin_addr.S_un.S_addr == INADDR_NONE) {
		puts("Error in converting ip address");
		exit(-1);
	}
}

//read file lines, returning a pointer to pointer
path_t** read_lines(FILE* conf_file) {
	//path structure array
	path_t** array;
	//array index-iterator
	int i, j, byte_read, line_size = 100, num_lines = 100;
	//temp buffer for reading character
	char* temp;
	//initializing number of lines (100 lines)
	array = malloc(sizeof(path_t*)*100);
	//read file line by line
	for(i = 0; ; i++) {
		if(i > num_lines) {
			num_lines += 100;
			//realloc of array size
			realloc(array, sizeof(path_t*)*num_lines);
		}
		//repristinating line_size
		line_size = 100;
		//initializing path_t structure for corresponding line
		array[i] = malloc(sizeof(path_t));
		//initializing path variable inside the structure
		array[i]->path = malloc(sizeof(char)*line_size);
		//iterating over a line
		for(j = 0; ; j++) {
			//exceeding path buffer
			if(j > line_size) {
				//reallocate line size
				line_size += 100;
				realloc(array[i]->path, sizeof(char)*line_size);
			}
			//reading char by char
			byte_read = fread(array[i]->path+j, 1, 1, conf_file);
			//recursive path
			if(j == 0 && *(array[i]->path+j) == '+') {
				//print for debugging
				array[i]->recusive = true;
			}
			//if new line
			if(*(array[i]->path+j) == '\r') {
				//substituting end of string to new line
				array[i]->path[j] = '\0';
				//reading newline character
				byte_read = fread(temp, 1, 1, conf_file);
				//checking whether it's really newline char
				if(*temp == '\n') {
					break;
				}
				else {
					puts("Error while reading newline character");
					exit(-1);
				}
			}
			if(byte_read == 0) {
				//reached end of file
				return array;
			}
		}
	}
	//function should exit when reading no character
	//if it reaches this point, something went wrong
	printf("[FAIL]\n");
	puts("Error while parsing configuration file ");
	exit(-1);

}


#endif /* DEBUG_CLIENT_WINDOWS_MS_H_ */
