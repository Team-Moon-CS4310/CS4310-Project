#include <string>

enum HttpStatus {
	OK,
	NOT_FOUND,
	INTERNAL_ERR
};

// TODO look at better way to pass socketDescriptor. Probably just make a global variable accessable to everything once the process is forked.
// See https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages

class ResponseInfo {
private:
public:
	HttpStatus status;
	std::string headers;
	std::string body;
	int socketDescriptor;

	ResponseInfo(int socketDescriptor);
	~ResponseInfo();

	std::string buildResponse();
};