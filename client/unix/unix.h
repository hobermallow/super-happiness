/** socket.h
 *
 *  Created on: Aug 10, 2016
 *      Author: and22
 */

#ifndef DEBUG_CLIENT_UNIX_H_
#define DEBUG_CLIENT_UNIX_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socketvar.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stropts.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>
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

/**
 * routine to wait for incoming clients
 * and send them socket for listening
 * on server updates
 */
void server_socket_passing_routine(thread_arg* threadArg) {
    //thread local variables
    //client address
    struct sockaddr_un client_address;
    //client address size
    int size = sizeof(client_address);
    //connected socket
    int connected_socket;
   //thread inherits all parent's variables
    if(0 != listen(threadArg->unix_socket, 100)) {
        perror("Error while imposing socket listening condition: ");
        exit(-1);
    }

    //infinite loop to accept incoming clients
    while(1) {
        //accept a new client
        if(0 > (connected_socket = accept(threadArg->unix_socket, (struct sockaddr *)&client_address, &size))) {
            //error while accepting incoming connection
            perror("Error while accepting incoming client connection: ");
            exit(-1);
        }
        //send to listen on server update
    }
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

    //struct for thread arguments
    thread_arg threadArg;
    //pipe variable
    int pipe;
    //value for setsockopt
	int value = 1;
	//bytes read
	int bytes, sender_size;
	//socket for server communication
	int sock;
    //UNIX DOMAIN SOCKET
    int unix_socket;
    //client identifier
    int client_id;
    //global variable monitoring number of active clients on a host
    int* active_clients;
    int active_clients_des;
    key_t active_clients_key;
    //global variable monitoring number of clients who read a certain section of incoming data
    int* readers;
    int readers_des;
    key_t readers_key;
    //global vairable for first synchronization of the processes
    int interlocked_des;
    int* interlocked;
    key_t interlocked_key;
    //mutex variables
    int mutex_des;
    pthread_mutex_t* mutex;
    pthread_mutexattr_t* mutex_attr;
	//receiving buffer
	char buffer[4096];
	//client address info
	struct sockaddr_in client, sender;
    //unix domain socket address
    struct sockaddr_un unix_socket_addr;


    //client address
	client.sin_addr.s_addr = INADDR_ANY;
	client.sin_port = htons((unsigned int)9090);
	client.sin_family = AF_INET;


	//preventing error "Address already in use"
//	if(-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value))) {
//        perror("");
//    }




    //getting interlocked variable for Dekker's algorithm
    //getting file descriptor of shared memory location
    interlocked_des = shmget(INTERLOCKED, sizeof(int),   IPC_CREAT | IPC_EXCL | O_RDWR | S_IRWXG | S_IRWXU | S_IRWXO);

    //can fail also if memory is already allocated
    if(interlocked_des < 0) {
        //shared memory already exists
        interlocked_des = shmget(INTERLOCKED, sizeof(int), O_RDWR);
        if(interlocked_des < 0) {
            //just an error...
            perror("Error shmget: ");
        }
    }

    //mapping shared memory object in process space address
    interlocked = shmat(interlocked_des, NULL, 0);
    if(interlocked == -1) {
        perror("Error shmat: ");
    }

    //BEGINNING EXCLUSIVE SECTION
    //exclusive section for mutex initializing and client identifying
    while(__sync_lock_test_and_set(interlocked, 1) == 1) {
        //loooooooooooooooooooop
    }
    printf("Getting clients number... ");

    //getting shared variables indicating number of clients running on the system
    active_clients_des = shmget(CLIENTS, sizeof(int), IPC_EXCL | IPC_CREAT | O_RDWR | S_IRWXG | S_IRWXU | S_IRWXO);

    //failing due to already intialized memory
    if(active_clients_des < 0) {
        active_clients_des = shmget(CLIENTS, sizeof(int), O_RDWR);
        if(active_clients_des < 0) {
            //just an error...
            perror("Error shmget: ");
        }
    }

    
    //getting number of already active clients
    active_clients = shmat(active_clients_des, NULL, 0);

    //client identifier
    client_id = ++(*active_clients);

    puts("[OK]");
    //as result of truncating , if this is the first client, active_clients variables should be equal to 0

    printf("Client id %d\n", client_id);
    printf("Getting mutex... ");
    //opening mutex in shared memory
    mutex_des = shmget(MUTEX, sizeof(pthread_mutex_t), IPC_EXCL | IPC_CREAT | O_RDWR | S_IRWXG | S_IRWXU | S_IRWXO);

    //failing due to already intialized memory
    if(mutex_des < 0) {
        mutex_des = shmget(MUTEX, sizeof(pthread_mutex_t), O_RDWR);
        if(mutex_des < 0) {
            //just an error...
            perror("Error shmget: ");
        }
    }

    //getting number of already active clients
    mutex = shmat(mutex_des, NULL, 0);

    puts("[OK]");

    //mutex initializing SHOULD HAPPEN ONLY ONCE
    if(client_id == 1) {
        //only the first client should initialize the mutex
        puts("Initializing mutex, only first activated client");
        mutex_attr = malloc(sizeof(pthread_mutexattr_t));
        pthread_mutexattr_init(mutex_attr);
        pthread_mutexattr_setpshared(mutex_attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(mutex, mutex_attr);
        //creating socket for server communication
        sock = create_socket();
        //avoiding "Address already in use" error
        if(-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &value, sizeof(value))) {
            perror("");
        }

        //initializing UNIX DOMAIN SOCKET to share previously created socket
        if(0 > (unix_socket = socket(AF_UNIX, SOCK_STREAM, 0))) {
            perror("Error creating unix domain socket: ");
            (*interlocked) = 0;
            (*active_clients) = 0;
            exit(-1);
        }

        printf("Binding socket address... ");
        //binding socket to 9090 port and server address
        if(bind(sock,(struct sockaddr*) &client, sizeof(client)) != 0){
            perror("Binding error: ");
            exit(-1);
        }
        puts("[OK]");

        //cleaning socket address variable
        memset(&unix_socket_addr, 0, sizeof(unix_socket_addr));
        //setting local address
        unix_socket_addr.sun_family = AF_UNIX;
        strncpy(unix_socket_addr.sun_path, UNIX_SOCKET, sizeof(unix_socket_addr.sun_path));

        //unlinking path
        unlink(UNIX_SOCKET);

        if(-1 == bind(unix_socket, (struct sockaddr*) &unix_socket_addr, sizeof(unix_socket_addr))) {
            perror("Error while binding unix domain socket: ");
            exit(-1);
        }

        //thread variable
        pthread_t socket_thread;
        //assigning arguments to thread arguments structure
        threadArg.socket = sock;
        threadArg.unix_socket = unix_socket;
        //start new thread
        if(0 != pthread_create(&socket_thread, NULL, server_socket_passing_routine, &threadArg )) {
            //error while creating new thread
            perror("Error while creating thread to manage socket passing: ");
            exit(-1);
        }
    }
    //if this is not the first client
    else {

    }



    //initializing global variables for monitoring number of readers
    readers_des = shmget(READERS, sizeof(int), IPC_EXCL | IPC_CREAT | O_RDWR | S_IRWXG | S_IRWXU | S_IRWXO);

    if(readers_des < 0) {
        readers_des = shmget(READERS, sizeof(int), O_RDWR);
        if(readers_des < 0) {
            perror("Error while getting readers shared variable ");
            exit(-1);
        }

    }

    //getting number of already active clients
    readers = shmat(readers_des, NULL, 0);
    //END OF EXCLUSIVE SECTION
    (*interlocked) = 0;



    sender_size = sizeof(sender);

	while(1) {
        //mutexing on readers
        pthread_mutex_lock(mutex);
        //mutexing on active clients
        while(__sync_lock_test_and_set(interlocked, 1) == 1) {
            //loooooooooooooooooooop
        }
        //incrementing readers number
        (*readers)++;
        printf("Active clients: %d Readers: %d\n", *active_clients, *readers);
        if((*readers) == (*active_clients)) {
            //restarting readers counter
            (*readers) = 0;
            //emptying socket
            puts("Waiting for data");
            bytes = recvfrom(sock, buffer, 4096, 0, (struct sockaddr*)&sender, &sender_size);
            (*interlocked) = 0;
            pthread_mutex_unlock(mutex);
        }
        else {
            puts("Waiting for data");
            //peeking on socket
            bytes = recvfrom(sock, buffer, 4096, MSG_PEEK, (struct sockaddr*)&sender, &sender_size);
            (*interlocked) = 0;
            pthread_mutex_unlock(mutex);
        }

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
