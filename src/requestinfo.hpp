#include <string>

enum RequestMethod {
	GET,
	POST,
	DELETE,
	PUT
};

//TODO add the others?
// Thanks to https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9
// I honestly don't know if this goes here or in the .cpp file.
class RequestInfo {
private:
	std::string fullRequest;

public:
	int contentLength;
	int socketDescriptor;
	RequestMethod method;
	std::string path;
	std::string fileName;
	std::string body;
	std::string boundary;

	RequestInfo(std::string req, int sD);
	~RequestInfo();
	std::string getHost();
	std::string getBody();
	std::string getUserAgent();
	std::string getAccept();
	std::string getAcceptLanguage();
	std::string getAcceptEncoding();
	std::string getContentType();
	std::string getHeader(std::string name);
};
