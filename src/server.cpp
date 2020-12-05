#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include <iostream>

#include "clienthandler.hpp"
#include "colors.hpp"

pthread_mutex_t m;

int main(int argc, char const *argv[]) {
	addrinfo hint;
	addrinfo *info;

	memset(&hint, 0, sizeof hint);	// Required.
	hint.ai_family = AF_UNSPEC;		// Don't care if ipv4 or ipv6.
	hint.ai_socktype = SOCK_STREAM;	// Sets the socket to handle TCP instead of UDP. VERY IMPORTANT!
	hint.ai_flags = AI_PASSIVE;	 // Puts local host address in automatically.

	// Sets up the struct info.
	int result = getaddrinfo(NULL, "8080", &hint, &info);
	// Returns error code that can be decoded with specifically gai_strerror().
	if (result != 0) {
		std::cout << gai_strerror(result) << "\n";
	} else {
		printColor(GREEN, "Got address info successfully!");
	}

	// Now info points to a list of addrinfos with the given address and port.
	// We can usually assume that the first element is correct, as there shouldn't (usually) be any existing
	// addresses with the same port being used. LOOKAT

	// Makes a socket that can be dealt with using the file descriptor returned.
	int socketResult = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	// Handle error printing.
	if (socketResult == -1) {
		perror("socketResult");
	} else {
		printColor(GREEN, "Got socket successfully!");
	}

	// Thank you https://stackoverflow.com/questions/24194961/how-do-i-use-setsockoptso-reuseaddr
	// Make it so localhost is automatically reused. Dangerous if localhost is already being taken up.
	// Gets rid of 'address already taken' error when attempting to bind the socket.
	int on = 1;
	setsockopt(socketResult, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	// Actually allows us to use listen().
	int bindResult = bind(socketResult, info->ai_addr, info->ai_addrlen);
	// Handle error printing.
	if (bindResult == -1) {
		perror("bindResult");
	} else {
		printColor(GREEN, "Bound socket successfully!");
	}

	// Might not be best here. TODO LOOKAT
	freeaddrinfo(info);

	// Starts listening on port/address given for connections.
	// Places them in a queue, the size of which is the 2nd argument here.
	int listenResult = listen(socketResult, 2);
	// Handle error printing.
	if (listenResult == -1) {
		perror("listenResult");
	} else {
		printColor(GREEN, "Listening on socket successfully!");
	}

	// TODO look at select()
	// I get 2 connections because firefox requests the favcon.ico immediently.
	while (true) {
		// Make a new socket for the incoming client. I think?
		sockaddr_storage clientSocket;

		pthread_t thread;

		// Get the address's length.
		socklen_t clientAddrSize = sizeof clientSocket;
		int sD;

		printColor(YELLOW, "Waiting for Client. . .");
		// This is a blocking method. Waits for connection and automatically accepts it.
		// Makes a socket descriptor, which is actually a file descriptor.
		// Just is a reference to the socket buffer when used properly.
		sD = accept(socketResult, (sockaddr *)&clientSocket, &clientAddrSize);

		printColor(GREEN, "Client Accepted");

		// Error printing.
		if (sD == -1) {
			perror("acceptResult");
			continue;
		}

		pthread_create(&thread, NULL, handleClient, &sD);

		pthread_join(thread, NULL);
	}
	return 0;
}