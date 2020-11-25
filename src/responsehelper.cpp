#include "responsehelper.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <filesystem>
using namespace std;

/**
 * @brief Sends the response to the client via the socketDescriptor in the ResponseInfo reply.
 * 
 * @param reply The full ResponseInfo.
 * @return int whether the send succeeded or not.
 */
int sendResponse(ResponseInfo* reply) {
	// Send the header by default. We assume that it's small enough to not be a problem in the buffer.
	// AKA. it shouldn't be any greater than 1k bytes.

	filesystem::path p{reply->filePath};
	reply->fileSize = filesystem::file_size(p);
	string header = reply->buildHeader();
	cout << "header: " << header << endl;

	header.append(fileToString(reply->filePath));

	int s = send(reply->socketDescriptor, header.c_str(), header.size(), 0);
	if (s == -1) {
		perror("sendResponse");
	}



	//Now we want to send any files that should be sent.
	// fstream filestream(reply->filePath);
	// char* temp = new char[1024];

	// string sep = "\r\n";

	// while (filestream.read(temp, 1024)) {
	// 	int len = strlen(temp);

	// 	stringstream hexstream;
	// 	hexstream << hex << len;
	// 	string sizestring = hexstream.str();
	// 	sizestring.append("\r\n");

	// 	cout << "sizestring: " << len << endl;

	// 	s = send(reply->socketDescriptor, sizestring.c_str(), sizestring.size(), MSG_MORE);
	// 	if (s == -1) {
	// 		perror("sendResponse");
	// 	}

	// 	s = send(reply->socketDescriptor, temp, len, MSG_MORE);
	// 	if (s == -1) {
	// 		perror("sendResponse");
	// 	}

	// 	s = send(reply->socketDescriptor, sep.c_str(), sep.length(), MSG_MORE);
	// 	if (s == -1) {
	// 		perror("sendResponse");
	// 	}
	// 	for (int i = 0; i < 1024; i++) {
	// 		temp[i] = 0;
	// 	}
	// }
	// cout << "ending" << endl;
	// string endstuff = "0\r\n";
	// s = send(reply->socketDescriptor, endstuff.c_str(), 3, 0);
	// if (s == -1) {
	// 	perror("sendResponse");
	// }
	// delete[] temp;
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