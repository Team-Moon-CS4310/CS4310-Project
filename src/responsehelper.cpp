#include "responsehelper.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

/**
 * @brief Sends the response to the client via the socketDescriptor in the ResponseInfo reply.
 * 
 * @param reply The full ResponseInfo.
 * @return int whether the send succeeded or not.
 */
int sendResponse(ResponseInfo reply) {
	string content = reply.buildResponse();
	auto tosend = content.c_str();
	cout << content << endl;
	int s = send(reply.socketDescriptor, tosend, strlen(tosend), 0);
	if (s == -1) {
		perror("sendResponse");
	}
	return 0;
}

/**
 * @brief Converts the given file to a string by reading it from a stringstream.
 * 
 * @param filename the string filename.
 * @return string the contents of the file.
 */
string fileToString(string filename) {
	ifstream f(filename);
	stringstream buff;
	buff << f.rdbuf();
	return buff.str();
}