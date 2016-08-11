/*
 * main.c
 *
 *  Created on: Aug 10, 2016
 *      Author: and22
 */
#include <stdio.h>
#include <stdlib.h>
//#ifdef _WIN32
//#include "windows/ms.h"
//#else
#include "unix/unix.h"
//#endif

int main(int argc, char **argv) {
	if((sock = create_socket()) == -1) {
		puts("Error while creating socket");
		return -1;
	}
	return 0;
}

