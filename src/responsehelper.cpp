#include "responsehelper.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

string buildResponse(ResponseInfo info) {
	string result = "";
	result.append("HTTP/1.1 ");

	switch (info.status) {
	case OK:
		result.append("200 OK");
		break;
	case NOT_FOUND:
		result.append("404 Not Found"); // No clue why this isn't working properly...
		break;
	case INTERNAL_ERR:
		result.append("500 Internal Server Error");
		break;
	default:
		// Because I can, that's why.
		result.append("418 I'm a teapot");	// Short and stout.
		break;
	}
	if (!info.body.empty())
	{
		result.append("\n\n");
		result.append(info.body);
	}

	return result;
}

int sendResponse(ResponseInfo reply) {
	string content = buildResponse(reply);
	auto tosend = content.c_str();
	cout << content << endl;
	int s = send(reply.socketDescriptor, tosend, strlen(tosend), 0);
	if (s == -1) {
		perror("sendResponse");
	}
	return 0;
}

string fileToString(string filename) {
	ifstream f(filename);
	stringstream buff;
	buff << f.rdbuf();
	return buff.str();
}