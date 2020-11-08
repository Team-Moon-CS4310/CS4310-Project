#include "requestinfo.hpp"

#include <iostream>
#include <regex>
using namespace std;

/*
Parses HTTP request string. TODO docs.
See https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9 for layout of HTTP request format.
*/

/**
 * @brief Construct a new Request Info:: Request Info object
 * 
 * @param req The full string of recieved information from the client, including
 * the full header and whatnot.
 * See https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9
 * @param sD Socket Descriptor 
 */
RequestInfo::RequestInfo(string req, int sD) {
	socketDescriptor = sD;
	// Parse string similar to the terminal we made. Assignment 4.
	fullRequest = req;

	// Get basic things we will always need
	regex reg("(\\S+)");
	sregex_iterator next(req.begin(), req.end(), reg);

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
		method = GET;
	} else if (match.compare("POST") == 0) {
		method = POST;
		contentLength = stoi(getHeader("Content-Length"));
		cerr << "length: " << getHeader("Content-Length") << "\n";
	} else if (match.compare("DELETE") == 0) {
		method = DELETE;
	} else if (match.compare("PUT") == 0) {
		method = PUT;
	}

	nextMatch(0);
	path = match;
}

RequestInfo::~RequestInfo() {
}

// Chose to do this because we won't need 90% of the headers in the usual case unless we decide to implement them.
/**
 * @brief Finds the string following the inputted string name, starting 2 characters ahead.
 * So, 'Content-Length' would find everything after 'Content-Length: '
 * 
 * @param name The name of the header.
 * @return The found information after the given header.
 */
string RequestInfo::getHeader(string name) {
	size_t start = fullRequest.find(name);
	if (start == string::npos) {
		return NULL;
	}
	size_t endLine = fullRequest.find("\n", start);

	size_t startOfData = start + name.length() + 2;

	return fullRequest.substr(startOfData, endLine - startOfData);
}