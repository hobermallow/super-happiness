/*
 * main.c
 *
 *  Created on: Aug 10, 2016
 *      Author: and22
 */
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//#include <arpa/inet.h>
//if it's compiling under Windows
#ifdef _WIN32
#include "windows/ms.h"
//if it's compiling under unix
#else
#include "unix/unix.h"
#endif

//read configuration file
int create_configuration_file() {
	int fd;
	puts("Creating configuration file...");
	fd = open("happiness.conf", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if(fd == -1) {
		puts("Error in creating configuration file");
		perror("");
		exit(-1);
	}
	puts("Configuration file created");
	return fd;
}

//open configuration file
int open_configuration_file(char* path) {
	int fd;
	puts("Opening configuration file...");
	fd = open(path, O_RDWR);
	if(fd == -1) {
			puts("Error in reading configuration file");
			perror("");
			exit(-1);
	}
	puts("Configuration file read");
	return fd;
}

//converting dot notation ip address to struct sockaddr_in
void convert_ip_to_address(const char* ip, struct sockaddr_in* addr) {
	//converting ip address in dotted notation
	//to a suitable form for struct in_addr
	addr->sin_addr.S_un.S_addr =  inet_addr(ip);
	if(addr->sin_addr.S_un.S_addr == INADDR_NONE) {
		puts("Error in converting ip address");
		exit(-1);
	}
}

char** read_lines(int conf_file) {
	//lines array
	char** array;
	//array index-iterator
	int i, j, byte_read, line_size = 100, num_lines = 100;
	//initializing number of lines (100 lines)
	array = malloc(sizeof(char*)*100);
	//read file line by line
	for(i = 0; ; i++) {
		if(i > num_lines) {
			num_lines += 100;
			//realloc of array size
			realloc(array, sizeof(char*)*num_lines);
		}
		//initializing line size (100 characters)
		array[i] = malloc(sizeof(char)*100);
		//ripristinate line_size
		line_size = 100;
		//iterating over a line
		for(j = 0; ; j++) {
			if(j > line_size) {
				//reallocate line size
				line_size += 100;
				realloc(array[i], line_size);
			}
			//reading char by char
			byte_read = read(conf_file, array[i]+j, 100);
			//if new line
			if(byte_read == 0) {
				//reached end of file
				return array;
			}
		}
	}

}

void parse_configuration_file(int conf_file) {
	//read file
	read_lines(conf_file);
}


int main(int argc, char **argv) {

	//variables

	//client configuration file
	int conf_file;
	//host IP address
	struct sockaddr_in server;
	//array containing paths of interests
	char** paths;




	//read configuration file if any
	//or create one if it's not passed on
	//command line
	if(argc == 2) {
		convert_ip_to_address(argv[1], &server);
		//create new configuration file
		conf_file = create_configuration_file();
	}
	else if(argc == 3) {
		convert_ip_to_address(argv[1], &server);
		//read the old conf file
		conf_file = open_configuration_file(argv[2]);
	}
	else {
		puts("Wrong number of arguments");
		puts("Usage: super-happiness host port [conf-file]");
	}

	parse_configuration_file(conf_file);
	//socket creation
	//socket is a global variable, differently defined in windows and unix file
	if((sock = create_socket()) == -1) {
		puts("Error while creating socket");
		return -1;
	}


	return 0;
}

