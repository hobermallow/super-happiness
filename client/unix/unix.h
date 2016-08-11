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

int sock;

/**
 * @return int corresponding to socket
 */
int create_socket() {
	//defining socket fd
	int s;
	puts("Creating socket...");
	//create socket
	if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		//return -1 if socket is not created
		puts("Error while creating socket");
		exit(-1);
	}
	puts("Socket successfully created");
	//return value of socket fd
	return s;
}

#endif /* DEBUG_CLIENT_UNIX_H_ */
