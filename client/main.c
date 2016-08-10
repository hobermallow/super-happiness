/*
 * main.c
 *
 *  Created on: Aug 10, 2016
 *      Author: and22
 */
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include "windows/ms.h"
#else
#include "unix/unix.h"
#endif

int main(int argc, char **argv) {
	SOCKET s;
	if((s = create_socket()) != 0) {
		puts("Errore nella creazione del socket");
	}
	return 0;
}

