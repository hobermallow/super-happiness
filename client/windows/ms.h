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

SOCKET sock;

SOCKET create_socket() {
	//socket
	SOCKET sock;
	//initializing win socket structure
	WSADATA wsa;
	puts("Initializing windows socket structure...");
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		puts("Error in Windows socket structure initialization");
		exit(-1);
	}
	puts("Creating socket...");
	//create socket
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		puts("Error while creating socket...");
		exit(-1);
	}
	puts("Socket successfully created");
	//socket successfully created
	return sock;
}


#endif /* DEBUG_CLIENT_WINDOWS_MS_H_ */
