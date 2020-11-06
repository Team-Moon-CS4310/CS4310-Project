#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>
// LOOKAT jank. requesthandler.hpp included through requesthelper.hpp.
#include "requesthelper.hpp"

using namespace std;

// See https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages for response.
// Want to return base html page.
int getRequest(RequestInfo info) {
	ResponseInfo reply;
	cout << info.path << endl;

	if (info.path.compare("/") == 0) {
		// Set reply status.
		reply.status = OK;
		// Put the index.html file into the response body.
		reply.body = fileToString("res/index.html");
		reply.socketDescriptor = info.socketDescriptor;
		// Pass the response info to be built and sent.
		sendResponse(reply);
	} else if (info.path.compare("/favicon.ico") == 0) {
		reply.status = NOT_FOUND;
		reply.socketDescriptor = info.socketDescriptor;
		sendResponse(reply);
	}
	return 0;
}

int postRequest(RequestInfo info) {
	return 0;
}

int deleteRequest(RequestInfo info) {
	return 0;
}

int putRequest(RequestInfo info) {
	return 0;
}