#include "requesthandler.hpp"

RequestInfo parseRequest(std::string req);

std::string buildResponse(ResponseInfo info);

int sendResponse(ResponseInfo info);

string fileToString(string filename);