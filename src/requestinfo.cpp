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
		boundary = getHeader("boundary");
		//Need check since for some reason, recv() returns headers as first chunk
		//Subsequent chunks has the information we need
		//Remove once we implement recv() to return full request
		if (!getHeader("filename").empty()) {
			fileName = getHeader("filename");
		}
		if (!getHeader("body").empty()) {
			body = getHeader("body");
		}

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
	size_t start;
	size_t endLine;
	size_t startOfData;
	size_t minimum;

	if (name.compare("body") == 0) {
		minimum = fullRequest.find("filename");

		if (minimum == string::npos) {
			//Returns empty string
			//Error if return NULL when function returns string
			return "";
		} else {
			string printedBoundary = "";
			printedBoundary.append("-");
			printedBoundary.append("-");
			printedBoundary.append(boundary);

			cout << "Boundary: " << printedBoundary << endl;

			endLine = fullRequest.find(printedBoundary);

			while (endLine < minimum) {
				//Gets next iteration
				endLine = fullRequest.find(printedBoundary, endLine + 1);
			}
		}

		//"\r\n\r\n" is at the end of headers
		start = fullRequest.find("\r\n\r\n");
		//Gets the right boundary section of where we get the data
		while (start < minimum) {
			//Gets next iteration
			start = fullRequest.find("\r\n\r\n", start + 1);
		}
		startOfData = start + 4;
	} else {
		start = fullRequest.find(name);

		if (start == string::npos) {
			//Returns empty string
			//Error if return NULL when function returns string
			return "";
		}

		endLine = fullRequest.find("\n", start);

		startOfData = start + name.length();

		if (name.compare("Content-Length") == 0) {
			startOfData += 2;
		} else if (name.compare("boundary") == 0) {
			startOfData += 1;
		} else if (name.compare("filename") == 0) {
			startOfData += 2;
			endLine -= 2;
		}
	}

	return fullRequest.substr(startOfData, endLine - startOfData);
}
