#include "requesthelper.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

/*
Parses HTTP request string, convering it into a RequestInfo struct.
See https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9 for layout of HTTP request format.
*/
RequestInfo parseRequest(string req) {
	// Parse string similar to the terminal we made. Assignment 4.
	regex reg("(\\S+)");
	sregex_iterator next(req.begin(), req.end(), reg);

	RequestInfo result;
	smatch smatch = *next;
	sregex_iterator end;
	string match = smatch.str();

	// Lambda function can modify variables in the scope its declared.
	auto nextMatch = [&](int skip) {
		for (int i = 0; i <= skip; i++) {
			next++;
		}
		smatch = *next;
		match = smatch.str();
	};

	// Fill the request method.
	if (match.compare("GET") == 0) {
		result.method = GET;
	} else if (match.compare("POST") == 0) {
		result.method = POST;
	} else if (match.compare("DELETE") == 0) {
		result.method = DELETE;
	} else if (match.compare("PUT") == 0) {
		result.method = PUT;
	}

	nextMatch(0);
	result.path = match;

	nextMatch(2);
	result.host = match;

	return result;
}

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