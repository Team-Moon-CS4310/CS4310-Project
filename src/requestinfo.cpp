#include "requestinfo.hpp"

#include <iostream>
#include <regex>
using namespace std;

/*
Parses HTTP request string. TODO docs.
See https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9 for layout of HTTP request format.
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

string RequestInfo::getHeader(string name) {
	// regex lineReg("(.+\\n)+");
	// sregex_iterator lineNext(fullRequest.begin(), fullRequest.end(), lineReg);
	// sregex_iterator end;
	// smatch smat = *lineNext;
	// for (string match = smat.str(); lineNext != end; lineNext++, smat = *lineNext, match = smat.str()) {
	// }

	size_t start = fullRequest.find(name);
	if (start == string::npos) {
		return NULL;
	}
	size_t endLine = fullRequest.find("\n", start);

	size_t startOfData = start + name.length() + 2;

	return fullRequest.substr(startOfData, endLine - startOfData);
}