#include "requestinfo.hpp"

#include <fstream>
#include <iostream>
#include <regex>

using namespace std;

/**
 * @brief Construct a new Request Info:: Request Info object
 * 
 * @param head The header of the request and potentially a little data afterwards. 
 * AKA the first 1024 bytes.
 * See https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9
 * @param sD Socket Descriptor 
 */
RequestInfo::RequestInfo(string head, int sD, string requestFileName) {
	socketDescriptor = sD;
	// Parse string similar to the terminal we made. Assignment 4.
	fullRequest = head;

	// Get basic things we will always need
	regex reg("(\\S+)");
	sregex_iterator next(head.begin(), head.end(), reg);

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
		fileName = getHeader("filename");
		separateFile(requestFileName);

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

	return fullRequest.substr(startOfData, endLine - startOfData);
}

/**
 * @brief Separates the file from the frontend request if it's multipart/form-content ONLY.
 * Also, only separates the first file.
 * 
 * @param requestFileName 
 */
void RequestInfo::separateFile(string requestFileName) {
	string printedBoundary = "";
	printedBoundary.append("-");
	printedBoundary.append("-");
	printedBoundary.append(boundary);

	// Parse requestFile now to separate the body into a separate file.
	// Read line by line until we hit the boundary. Record the start and end of the file.
	fstream file(requestFileName);
	string fileLoc = "files/";
	fileLoc.append(fileName);
	ofstream fileWrite(fileLoc);
	string line;
	int stage = 0;
	while (getline(file, line)) {
		if (line.find(printedBoundary) != string::npos) {
			// Found the first boundary.
			// Discard next 3 lines.
			getline(file, line);
			getline(file, line);
			getline(file, line);
			stage = 1;
			break;
		}
	}
	if (stage == 1) {
		// Now we want to copy the bytes to a new file until we hit the boundary again.
		// TESTING
		while (getline(file, line)) {
			if (line.find(printedBoundary) != string::npos) {
				break;
			}
			line.append("\n");	// TODO look into. Is likely incorrect and might cause problems. But reading lines already removes the \n, so maybe it's okay?
			fileWrite.write(line.c_str(), line.length());
		}
	} else {
		cout << "Warning: no file parsed when expected!?" << endl;
	}

	file.close();
	fileWrite.close();
}