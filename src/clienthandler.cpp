#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "requesthandler.hpp"
using namespace std;

/*
Order of operations we want:
1. Check what request is
2. Send request (get, post, delete, or put) to it's appropriate handler (get, post, delete, or put)
3. That handler then takes the path and then passes it to its appropriate method to handle that specific request.
*/
// TODO handle 'Connection: keep-alive'
int handleClient(int sD) {
	cout << "Inside: " << sD << endl;

	char buffer[2000];
	int received = recv(sD, buffer, sizeof(buffer), 0);
	if (received == -1) {
		perror("received");
		cout << "ERROR RECEIVING!" << endl;
	}

	cerr << received << "\n";

	cerr << "\033[34m" << buffer << "\033[0m\n";

	string str(buffer);
	RequestInfo info(str, sD);

	switch (info.method) {
	case GET:
		getRequest(info);
		break;
	case POST:
		postRequest(info);
		break;
	case DELETE:
		deleteRequest(info);
		break;
	case PUT:
		putRequest(info);
		break;
	default:
		auto errmsg = "Request not cannot be handled.";
		send(sD, errmsg, strlen(errmsg), 0);
		break;
	}

	cerr << "done\n";
	return 0;
}