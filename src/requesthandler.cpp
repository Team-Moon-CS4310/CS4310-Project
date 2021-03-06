#include <sys/types.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>

#include "colors.hpp"
#include "requestinfo.hpp"
#include "responsehelper.hpp"

using namespace std;
extern pthread_mutex_t lock;

string createHTML();
bool existsIn(string base, string looking);
string toJSON(list<string> arr);
string getCurrentFileResponse();

// See https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages for response.
// Want to return base html page.
/**
 * @brief Handles GET requests.
 * 
 * @param info The client RequestInfo
 * @return int whether the send response succeeded.
 */
int getRequest(RequestInfo info) {
	// Print diagnostic stuff.
	string s = "GET: '";
	s.append(info.path).append("' CLIENT: ").append(to_string(pthread_self()));
	printColor(BLUE, s.c_str());

	ResponseInfo reply(info.socketDescriptor);

	if (info.path.compare("/") == 0) {
		// Set reply status.
		reply.status = OK;
		// Set file path to new html file
		reply.contentType = HTML;
		reply.filePath = "res/index.html";
		// Pass the response info to be built and sent.
		sendResponse(&reply);
	} else if (info.path.compare("/favicon.ico") == 0) {
		reply.status = NOT_FOUND;
		sendResponse(&reply);
	} else if (existsIn(info.path, "/css") || existsIn(info.path, "/js")) {
		string pathName = "res";  // This is where our static files are stored.
		pathName.append(info.path);	 // Make the full local path.
		if (existsIn(info.path, "/css")) {
			reply.contentType = CSS;
		} else {
			reply.contentType = JS;
		}

		// Thanks to https://stackoverflow.com/questions/59022814/how-to-check-if-a-file-exists-in-c
		ifstream check(pathName);
		if (check.is_open()) {
			reply.status = OK;
			reply.filePath = pathName;
		} else {
			reply.status = NOT_FOUND;
		}
		sendResponse(&reply);

	} else if (existsIn(info.path, "/file")) {
		string fileName = info.path.substr(5, info.path.size());  // Extract the stuff after /file/
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
	} else if (existsIn(info.path, "/getAllFiles")) {

		reply.status = OK;
		reply.contentType = JSON;
		reply.filePath = getCurrentFileResponse();
		sendResponse(&reply);
		remove(reply.filePath.c_str());
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
	// Print diagnostic stuff.
	string s = "POST: '";
	s.append(info.path).append("' CLIENT: ").append(to_string(pthread_self()));
	printColor(BLUE, s.c_str());

	ResponseInfo reply(info.socketDescriptor);

	if (info.path.compare("/upload") == 0) {
		reply.status = OK;
		// Send back the current existing files.
		reply.contentType = JSON;
		reply.filePath = getCurrentFileResponse();
		sendResponse(&reply);
		remove(reply.filePath.c_str());
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
	// Print diagnostic stuff.
	string s = "DELETE: '";
	s.append(info.path).append("' CLIENT: ").append(to_string(pthread_self()));
	printColor(BLUE, s.c_str());

	ResponseInfo reply(info.socketDescriptor);

	string pathName = "files/";	 // This is where our files are stored.
	pathName.append(info.path);	 // Make the full local path.

	// Thanks to https://stackoverflow.com/questions/59022814/how-to-check-if-a-file-exists-in-c
	ifstream check(pathName);
	if (check.is_open()) {
		reply.status = OK;
		pthread_mutex_lock(&lock);
		filesystem::remove(pathName.c_str());  // Delete the file.
		pthread_mutex_unlock(&lock);
	} else {
		reply.status = NOT_FOUND;
	}

	// Send back the current existing files.
	reply.contentType = JSON;
	reply.filePath = getCurrentFileResponse();
	sendResponse(&reply);
	remove(reply.filePath.c_str());
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

string createHTML() {
	// Set the response html file to be sent after the header.
	string fileName = "res/response";
	// Sets unique response html file name.
	fileName.append(to_string(pthread_self()));
	fileName.append(".html");
	// Write to html file (Thanks to https://stackoverflow.com/questions/11206604/create-html-reports-using-c)
	ofstream myFile;
	myFile.open(fileName);
	myFile << "<!DOCTYPE html><html>\n";
	myFile << "<style>body{background-color:lightblue;}\nul{list-style-type:none;padding-left:0px;}\nli{padding:10px;background-color:beige;}\n </style>\n";
	myFile << "<head>\n<meta charset='utf-8'>\n<meta http-equiv='X-UA-Compatible' content='IE=edge'>\n<title>Team Moon</title>\n<meta name='viewport' content='width=device-width, initial-scale=1'></head>\n";
	myFile << "<body>\n<form action=\"\" method=\"post\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"uploadfilevalue\" id=\"uploadfileid\"><input type=\"submit\" value=\"uploadfile\" name=\"submitfile\"> </form>\n";
	myFile << "<h5>Uploaded Files</h5>";
	myFile << "<iframe name=\"dontshowframe\" id=\"dontshowframe\" style=\"display: none;\"></iframe>";
	myFile << "\n<ul>";
	// Iterates through files and link with a button
	for (auto& p : filesystem::directory_iterator("files")) {
		myFile << "\n<a href=" << p.path() << " target=\"_blank\"><li><button type=\"button\">" << p.path().string().substr(6, p.path().string().length()) << "</button></li></a>\n";
		myFile << "<form action=\"delete/" << p.path().string().substr(6, p.path().string().length()) << "\" method=\"post\" target=\"_self\">";
		myFile << "<input value=\"Delete\" type=\"submit\">";
		myFile << "</form>";
	}
	myFile << "</ul></body></html>";
	myFile.close();
	// Return HTML file path
	return fileName;
}

string toJSON(list<string> arr) {
	string output = "[";
	int size = arr.size();
	int i = 0;
	for (string s : arr) {
		if (i == size - 1) {
			output.append("\"").append(s).append("\"");
		} else {
			output.append("\"").append(s).append("\"").append(",");
		}
		i++;
	}
	output.append("]");

	string fileName = "binaries/response";
	// Sets unique response html file name.
	fileName.append(to_string(pthread_self()));
	fileName.append(".json");
	// Write to html file (Thanks to https://stackoverflow.com/questions/11206604/create-html-reports-using-c)
	ofstream myFile;
	myFile.open(fileName);
	myFile << output;
	myFile.close();
	return fileName;
}

bool existsIn(string base, string looking) {
	return (base.find(looking) != string::npos);
}

string getCurrentFileResponse() {
	list<string> arr;
	for (auto& p : filesystem::directory_iterator("files")) {
		arr.push_back(p.path().filename());
	}
	return toJSON(arr);
}