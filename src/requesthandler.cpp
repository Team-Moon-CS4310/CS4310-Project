#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>

#include "requestinfo.hpp"
#include "responsehelper.hpp"

using namespace std;

// See https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages for response.
// Want to return base html page.
/**
 * @brief Handles GET requests.
 * 
 * @param info The client RequestInfo
 * @return int whether the send response succeeded.
 */
int getRequest(RequestInfo info) {
	ResponseInfo reply(info.socketDescriptor);
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

/**
 * @brief Handles POST requests.
 * 
 * @param info The client RequestInfo
 * @return int whether the send response succeeded.
 */
int postRequest(RequestInfo info) {
	//We can remove once recv() returns full request
	if (!info.fileName.empty() && !info.body.empty()) {
		std::fstream fileToStore = std::fstream(info.fileName, std::ios::out | std::ios::binary);
		fileToStore << info.body;
		fileToStore.close();
	}
	return 0;
}

/**
 * @brief Handles DELETE requests.
 * 
 * @param info The client RequestInfo
 * @return int whether the send response succeeded.
 */
int deleteRequest(RequestInfo info) {
	return 0;
}

/**
 * @brief Handles PUT requests.
 * 
 * @param info The client RequestInfo
 * @return int whether the send response succeeded.
 */
int putRequest(RequestInfo info) {
	return 0;
}
