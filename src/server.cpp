#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

void *handleClient(void *socketDescriptor);
struct pthreadargs {
	int socketDescriptor;
};

pthread_mutex_t m;

int main(int argc, char const *argv[]) {
	addrinfo hint;
	addrinfo *info;

	memset(&hint, 0, sizeof hint);	// Required.
	hint.ai_family = AF_UNSPEC;		// Don't care if ipv4 or ipv6.
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;	 // Puts local host address in automatically.

	int result = getaddrinfo(NULL, "8080", &hint, &info);  // Sets up the struct info.
	// Returns error code that can be decoded with specifically gai_strerror().
	if (result != 0) {
		std::cout << gai_strerror(result) << "\n";
	} else {
		std::cout << "Got address info successfully!" << std::endl;
	}

	// Now info points to a list of addrinfos with the given address and port.
	// We can usually assume that the first element is correct, as there shouldn't (usually) be any existing addresses with the same port being used.

	int socketResult = socket(info->ai_family, info->ai_socktype, info->ai_protocol);  // Might not want to assume first index of info is correct to use.
	// Now socketResult points to a file descriptor.
	if (socketResult == -1) {
		perror("socketResult");
	} else {
		std::cout << "Got socket successfully!" << std::endl;
	}

	// Actually allows us to use listen().
	int bindResult = bind(socketResult, info->ai_addr, info->ai_addrlen);
	// Returns potential error.
	if (bindResult == -1) {
		perror("bindResult");
	} else {
		std::cout << "Bound socket successfully!" << std::endl;
	}

	// Starts listening on port/address given for connections. Places them in a queue, the size of which is the 2nd argument here.
	int listenResult = listen(socketResult, 2);
	// Returns potential error.
	if (listenResult == -1) {
		perror("listenResult");
	} else {
		std::cout << "Listening on socket successfully!" << std::endl;
	}

	// TODO look at select()
	while (true) {
		pthread_t clientThread;
		sockaddr_storage clientSocket;					 // make a new socket for the incoming client.
		socklen_t clientAddrSize = sizeof clientSocket;	 // get the address's length.
		pthreadargs testargs;
		memset(&testargs, 0, sizeof testargs);
		std::cout << "Waiting for Client." << std::endl;
		testargs.socketDescriptor = accept(socketResult, (sockaddr *)&clientSocket, &clientAddrSize);
		// Returns a new socket descriptor that links to this newly accepted connection.
		if (testargs.socketDescriptor == -1) {
			perror("acceptResult");
			continue;
		}
		std::cout << "After: " << testargs.socketDescriptor << std::endl;

		pthread_create(&clientThread, NULL, handleClient, &testargs);

		pthread_join(clientThread, NULL);
	}

	freeaddrinfo(info);
	return 0;
}

void *handleClient(void *socketDescriptor) {
	pthreadargs *testargs = (pthreadargs *)socketDescriptor;
	std::cout << "Inside: " << testargs->socketDescriptor << std::endl;
	
	auto testmsg = "testmsg";
	int sendResult = send(testargs->socketDescriptor, testmsg, strlen(testmsg), 0);
	if (sendResult == -1) {
		perror("sendResult");
	}

	std::cout << "Closing" << std::endl;
	close(testargs->socketDescriptor);
	std::cout << "Closed" << std::endl;
	return NULL;
}