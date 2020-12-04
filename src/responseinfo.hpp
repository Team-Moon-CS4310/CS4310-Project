#include <fstream>
#include <string>

enum HttpStatus {
	OK,
	NOT_FOUND,
	INTERNAL_ERR
};

enum ContentType {
	JSON,
	HTML,
	TXT,
	UNDEFINED,
	JS,
	CSS
};

// TODO look at better way to pass socketDescriptor. Probably just make a global variable accessable to everything once the process is forked.
// See https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages

class ResponseInfo {
private:

public:
	HttpStatus status;
	std::string filePath;
	ContentType contentType;

	int socketDescriptor;

	ResponseInfo(int socketDescriptor);
	~ResponseInfo();

	std::string buildHeader();

};