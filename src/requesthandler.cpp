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
	reply.socketDescriptor = info.socketDescriptor;

	if (info.path.compare("/") == 0) {
		// Set reply status.
		reply.status = OK;
		// Set the index.html file to be sent after the header.
		reply.filePath = "res/index.html";
		// Pass the response info to be built and sent.
		sendResponse(&reply);
	} else if (info.path.compare("/favicon.ico") == 0) {
		reply.status = NOT_FOUND;
		sendResponse(&reply);
	} else if (info.path.find("/file") != string::npos) {
		string fileName = info.path.substr(6, info.path.size());  // Extract the stuff after /file/
		string pathName = "files/";	 // This is where our files are stored.
		pathName.append(fileName);	// Make the full local path.

		// Thanks to https://stackoverflow.com/questions/59022814/how-to-check-if-a-file-exists-in-c
		ifstream check(pathName);
		if (check.is_open()) {
			reply.status = OK;
			reply.filePath = pathName;
		} else {
			reply.status = NOT_FOUND;
		}
		sendResponse(&reply);
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
	if (info.path.compare("/upload") == 0) {
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
