#include "responsehelper.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <regex>

#include "colors.hpp"
using namespace std;
pthread_mutex_t lock;
/**
 * @brief Sends the response to the client via the socketDescriptor in the ResponseInfo reply.
 * 
 * @param reply The full ResponseInfo.
 * @return int whether the send succeeded or not.
 */
int sendResponse(ResponseInfo* reply) {
	string header = reply->buildHeader();
	printColor(MAGENTA, header.c_str());
	int s = send(reply->socketDescriptor, header.c_str(), header.size(), 0);
	if (s == -1) {
		printColor(RED, "Failed to send header!");
		perror("sendResponse");
	}

	// Only send a file if the path has been set.
	pthread_mutex_lock(&lock);
	char buffer[2048 * 2];
	if (!reply->filePath.empty()) {
		fstream filestream(reply->filePath);
		while (!filestream.eof()) {
			filestream.read(buffer, sizeof(buffer));
			s = send(reply->socketDescriptor, buffer, sizeof(buffer), 0);
			if (s == -1) {
				perror("sendResponse");
			}
		}
	}
	pthread_mutex_unlock(&lock);

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