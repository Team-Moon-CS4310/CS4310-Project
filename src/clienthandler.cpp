#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
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
/**
 * @brief Handles the client right after the server accepts it.
 * 
 * @param sD The socket descriptor made when the client was accepted.
 * @return int the status code.
 */
int handleClient(int sD) {
	// TODO move away from in-memory. Switch to buffer everything.
	/* 
	WARN anything that has to do with normal string manipulation will likely break the request data.
	Avoid at all cost in this method.
	*/
	// 1024 should be enough bytes for the header.
	char *header = new char[1024];
	int recvtotal = 0;
	char buffer[1024];

	string fileName = "binaries/request";
	int pid = getpid();
	cout << "pid: " << to_string(pid) << endl;
	fileName.append(to_string(pid));
	fileName.append(".bin");

	cout << "filename: " << fileName << endl;

	ofstream fileToStore(fileName);

	// Got help from https://github.com/iafonov/cosmonaut/blob/2bec53d0fff1c59d1e4089b582c910cbf8b9fd5c/src/net.c#L101
	fd_set sDset;
	timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	FD_ZERO(&sDset);
	FD_SET(sD, &sDset);
	int ready = select(sD + 1, &sDset, NULL, NULL, &tv);

	while (ready > 0) {
		memset(buffer, 0, sizeof(buffer));
		int received = recv(sD, buffer, sizeof(buffer), 0);
		if (received == -1) {
			perror("received");
			cout << "ERROR RECEIVING!" << endl;
		} else if (received == 0) {
			cout << "ZERO DATA?" << endl;
			return 0;
		}

		fileToStore.write(buffer, received);

		// We HAVE TO manipulate this data like this, there's no way around it.
		if (recvtotal == 0) {
			for (int i = 0; i < received; i++) {
				header[i + recvtotal] = buffer[i];
			}
		}
		
		recvtotal += received;

		ready = select(sD + 1, &sDset, NULL, NULL, &tv);
		cout << "selectin: " << ready << endl;
	}

	fileToStore.close();

	string str(header);
	delete[] header;
	RequestInfo info(str, sD, fileName);

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
	// Remove the request file for this specific client.
	cout << "removing " << fileName << endl;
	remove(fileName.c_str());

	cerr << "Done with client\n";
	return 0;
}