#include <string>

enum RequestMethod {
	GET,
	POST,
	DELETE,
	PUT
};

//TODO add the others?
// Thanks to https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9
struct RequestInfo {
	RequestMethod method;
	std::string host;
	std::string path;
	std::string body;
};

RequestInfo parseRequest(std::string req);