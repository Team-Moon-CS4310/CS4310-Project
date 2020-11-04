#include <string>

enum RequestMethod {
	GET,
	POST,
	DELETE,
	PUT
};

enum HttpStatus {
	OK,
	NOT_FOUND,
	INTERNAL_ERR
};

//TODO add the others?
// Thanks to https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9
struct RequestInfo {
	RequestMethod method;
	std::string host;
	std::string path;
	std::string body;
	int socketDescriptor;
};

// TODO look at better way to pass socketDescriptor. Probably just make a global variable accessable to everything once the process is forked.
// See https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
struct ResponseInfo {
	HttpStatus status;
	std::string headers;
	std::string body;
	int socketDescriptor;
};

RequestInfo parseRequest(std::string req);

int getRequest(RequestInfo info);

int postRequest(RequestInfo info);

int deleteRequest(RequestInfo info);

int putRequest(RequestInfo info);

std::string buildResponse(ResponseInfo info);

int sendResponse(ResponseInfo info);