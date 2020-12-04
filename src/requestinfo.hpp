#include <string>

enum RequestMethod {
	GET,
	POST,
	DELETE,
	PUT
};

/*
Parses HTTP request string.
See https://medium.com/better-programming/the-anatomy-of-an-http-request-728a469ecba9 for layout of HTTP request format.
*/

/**
 * @brief Handles the full http request received from clienthandler.hpp. Splits the request up and makes it easier to manage later on.
 * Additionally, parses the file from the request.
 * 
 */
class RequestInfo {
private:
	/**
	 * @brief The full http request received.
	 * 
	 */
	std::string fullRequest;

public:
	int contentLength;
	int socketDescriptor;
	RequestMethod method;
	std::string path;
	/**
	 * @brief The file to be sent to the client.
	 * 
	 */
	std::string fileName;
	std::string body;
	std::string boundary;

	RequestInfo(std::string head, int sD, std::string requestFileName);
	~RequestInfo();
	std::string getHost();
	std::string getBody();
	std::string getUserAgent();
	std::string getAccept();
	std::string getAcceptLanguage();
	std::string getAcceptEncoding();
	std::string getContentType();
	std::string getHeader(std::string name);
	void separateFile(std::string requestFileName);
};
