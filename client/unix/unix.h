/** socket.h
 *
 *  Created on: Aug 10, 2016
 *      Author: and22
 */

#ifndef DEBUG_CLIENT_UNIX_H_
#define DEBUG_CLIENT_UNIX_H_

#include <sys/socket.h>

/**
 * @return int corresponding to socket
 */
SOCKET* create_socket() {
	//define socket
	SOCKET s;
	//create socket
	if((s = socket(AF_INET, SOCK_STREAM, 0)) != 0) {
		//return -1 if socket is not created
		return -1;
	}
	//return reference to socket
	return &s;
}

#endif /* DEBUG_CLIENT_UNIX_H_ */
