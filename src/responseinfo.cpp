#include "responseinfo.hpp"

using namespace std;

/**
 * @brief Builds the full response string to be directly sent to the client via send().
 * 
 * @return string full response, in including HTTP/1.1, the status code, and the body.
 */
string ResponseInfo::buildResponse() {
	string result = "";
	result.append("HTTP/1.1 ");

	switch (status) {
	case OK:
		result.append("200 OK");
		break;
	case NOT_FOUND:
		result.append("404 Not Found");	 // No clue why this isn't working properly...
		break;
	case INTERNAL_ERR:
		result.append("500 Internal Server Error");
		break;
	default:
		// Because I can, that's why.
		result.append("418 I'm a teapot");	// Short and stout.
		break;
	}
	if (!body.empty()) {
		result.append("\n\n");
		result.append(body);
	}

	return result;
}

/**
 * @brief Construct a new ResponseInfo:: ResponseInfo object
 * 
 * @param socketDescriptor 
 */
ResponseInfo::ResponseInfo(int socketDescriptor) {
	this->socketDescriptor = socketDescriptor;
}

ResponseInfo::~ResponseInfo() {
}
