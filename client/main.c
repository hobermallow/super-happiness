/*
 * main.c
 *
 *  Created on: Aug 10, 2016
 *      Author: and22
 */
#include <stdio.h>
#include <stdlib.h>
//#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
//#include <arpa/inet.h>
//if it's compiling under Windows
#ifdef _WIN32
#include "windows/ms.h"
//if it's compiling under unix
#else
#include "unix/unix.h"
#endif

//header file for personal structures
#include "utils.h"

//read configuration file
FILE* create_configuration_file() {
	FILE* fd;
	printf("Creating configuration file... ");
	fd = fopen("happiness.conf", "w+");
	if(fd == NULL) {
		puts("Error in creating configuration file");
		perror("");
		exit(-1);
	}
	puts("[OK]");
	return fd;
}

//open configuration file
FILE* open_configuration_file(char* path) {
	FILE* fd;
	printf("Opening configuration file... ");
	fd = fopen(path, "a+");
	if(fd == NULL) {
			puts("Error in opening configuration file");
			perror("");
			exit(-1);
	}
	puts("[OK]");
	return fd;
}



conf_file_t* parse_configuration_file(FILE* conf_file) {
	//pointer to conf_file_t struct
	conf_file_t* paths;
	//read file
	printf("Parsing configuration file... ");
	//getting conf file structure
	paths = read_lines(conf_file);
	puts("[OK]");
	return paths;
}

void print_usage() {
	puts("Wrong number of arguments");
	puts("Usage: super-happiness  -w | -r path | -R path | -a path | -A path | -d path   host  port [conf-file]");
}

//check if the option selected for the command is a valid option
int check_option_validity(char* option) {
	if(strcmp(option, "-a") == 0 || strcmp(option, "-A") == 0 || strcmp(option, "-r") == 0 || strcmp(option, "-R") == 0 || strcmp(option, "-d") == 0 ) {
		return 1;
	}
	else {
		return 0;
	}
}


//check if the option selected needs a recursive behavior
int check_option_recursivity(char* option) {
	if(strcmp(option, "-a") == 0 || strcmp(option, "-r") == 0  ) {
		return 1;
	}
	else {
		return 0;
	}
}

//function to add a registration line in configuration file
void add_registration_to_conf_file(char* option, char* path, FILE* file_desc, conf_file_t* paths) {
	//file is already opened, just add a new line
	//first, we need to know string length
	//index for iterating
	int i;
	//byte read
	int byte_read;
	//temp buffer for reading chars
	char temp;
	//counting path string characters
	for(i = 0; ;i++) {
		//iterating while getting end of string
		temp = path[i];
		//if reached end of string
		if(temp == '\0') {
			break;
		}
	}

	//checking whether path is already registered
	if(check_path_already_registered(paths, path, option)) {
		puts("Path already registered");
		exit(-1);
	}
	//else
	printf("Adding path to configuration file... ");
	//if recursive option
	if(check_option_recursivity(option)) {
		//writing a '+' before paht string
		byte_read = fprintf(file_desc, "+");
		if(byte_read != 1) {
			printf("[FAIL]\n");
			puts("Error while writing '+' into configuration file");
			exit(-1);
		}
	}
//	printf("%d\n", i);
	//i contains characters number
	byte_read = fwrite(path, 1, i, file_desc);
	if(byte_read != i) {
		printf("[FAIL]\n");
		puts("Error while writing path into configuration file");
		exit(-1);
	}
	//writing EOL (PORTABLE)
	byte_read = fprintf(file_desc, "\n");
//	printf("%d\n", byte_read);
	if(byte_read != 1) {
		printf("[FAIL]\n");
		puts("Error while writing EOL into configuration file");
		exit(-1);
	}
	puts("[OK]");
}

int check_path_already_registered(conf_file_t* paths, char* path, char* option) {
	//index
	int i;
	//iterating over paths already registered
	for(i = 0; i < paths->num_paths; i++) {
		if(strcmp(path, paths->paths[i]->path) == 0 && paths->paths[i]->recusive == check_option_recursivity(option)) {
			//path found
			return 1;
		}
	}
	//if no corrispondence is found
	return 0;
}


int main(int argc, char **argv) {

	//variables

	//client configuration file
	FILE* conf_file;
	//server address
	struct sockaddr_in server;
	//array containing paths of interests
	conf_file_t* paths;

	//read configuration file if any
	//or create one if it's not passed on
	//command line
	if(argc == 4) {
		//waiting without configuration file
		if(strcmp(argv[1], "-w") == 0) {
			puts("Cannot wait for updates without configuration file");
			return -1;
		}
		//else, error in usage
		print_usage();
		return -1;
	}
	else if(argc == 5) {
		//waiting for updates showing on terminal
		if(strcmp(argv[1], "-w") == 0) {
			//setting server address
			convert_ip_to_address(argv[2], &server);
			server.sin_port = htons(atoi(argv[3]));
			//opening configuration file
			conf_file = open_configuration_file(argv[4]);
			//parsing configuration file
			paths = parse_configuration_file(conf_file);
			//entering monitor mode, updates on screen
			monitor_updates(paths, server);
		}
		//else, new registration with a new file
		//Checking option validity
		else if(check_option_validity(argv[1])) {
			convert_ip_to_address(argv[3], &server);
			server.sin_port = htons(atoi(argv[4]));
			//create new configuration file called happiness.conf
			conf_file = create_configuration_file();
			//parsing configuration file
			paths = parse_configuration_file(conf_file);
			//checking whether path to add already belongs to configuration file
			if(check_path_already_registered(paths, argv[2], argv[1])) {
				puts("Path already present in configuration file");
				exit(-1);
			}

			//ASK SERVER TO ADD PATH TO CONTROLLED PATH AND/OR REGISTER FOR PATH UPDATES

            //create socket to send request
            send_socket = create_socket();
            //send request
            send_registration_request(send_socket, server, argv[1], argv[2]);



			//if server accepted registration, we add path to configuration file
			//adding new registration to the newly created file
			add_registration_to_conf_file(argv[1], argv[2], conf_file, paths);
			//parsing configuration file again after adding new path
			paths = parse_configuration_file(conf_file);
		}
		//else, wrong usage (option)
		else {
			print_usage();
			return -1;
		}
	}
	else if(argc == 6) {
		//new registration added to an existing configuration file
		//checking validity of selected option
		if(check_option_validity(argv[1])) {
			//saving server infos
			convert_ip_to_address(argv[3], &server);
			server.sin_port = htons(atoi(argv[4]));
			//opening existing configuration file
			conf_file = open_configuration_file(argv[5]);
			//parsing configuration file
			paths = parse_configuration_file(conf_file);
			//checking whether path to add already belongs to configuration file
			if(check_path_already_registered(paths, argv[2], argv[1])) {
				puts("Path already present in configuration file");
				exit(-1);
			}

			//ASK SERVER TO ADD PATH TO CONTROLLED PATH AND/OR REGISTER FOR PATH UPDATES

			//adding registration to existing configuration file
			add_registration_to_conf_file(argv[1], argv[2], conf_file, paths);
			//parsing configuration file after adding new path
			paths = parse_configuration_file(conf_file);
		}
		else {
			//error in selected option
			print_usage();
			return -1;
		}
	}

	else {
		//invalid usage
		print_usage();
		return -1;
	}

	//setting file seeker at beginning of file
//	fseek(conf_file, 0, SEEK_SET);


	//closing configuration file
	if(fclose(conf_file) != 0) {
		perror("");
		return -1;
	}

	//socket creation
	//socket is a global variable, differently defined in windows and unix file
//	if((sock = create_socket()) == -1) {
//		puts("Error while creating socket");
//		return -1;
//	}
//	//determining server address
//	server.sin_family = AF_INET;
//	server.sin_port = htons(atoi(argv[2]));
//	//connecting to server
//	if(connect(sock, &server, sizeof(server)) != 0) {
//		perror("Error while connecting to server");
//		return -1;
//	}

	return 0;
}

