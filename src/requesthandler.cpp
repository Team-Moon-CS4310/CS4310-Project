#include "requesthandler.hpp"

#include <regex>
using namespace std;

RequestInfo parseRequest(string req) {
	regex reg("(\\S+)");
	sregex_iterator next(req.begin(), req.end(), reg);

	RequestInfo result;
	smatch smatch = *next;
	sregex_iterator end;
	string match = smatch.str();

	auto nextMatch = [&](int skip) {
		for (int i = 0; i <= skip; i++) {
			next++;
		}
		smatch = *next;
		match = smatch.str();
	};

	// Fill the request method.
	if (match.compare("GET") == 0) {
		result.method = GET;
	} else if (match.compare("POST") == 0) {
		result.method = POST;
	} else if (match.compare("DELETE") == 0) {
		result.method = DELETE;
	} else if (match.compare("PUT") == 0) {
		result.method = PUT;
	}

	nextMatch(0);
	result.path = match;

	nextMatch(2);
	result.host = match;

	return result;
}