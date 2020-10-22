#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char const *argv[]) {
	addrinfo hint;
	addrinfo *info;

	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;

	int result = getaddrinfo(NULL, "8080", &hint, &info);


	return 0;
}
