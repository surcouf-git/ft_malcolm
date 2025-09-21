#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[]) {
	struct addrinfo hints, *res = 0, *ptr = 0;
	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int result = getaddrinfo("www.google.com", NULL, &hints, &res);

	ptr = res;
	while (ptr) {
		printf("info:%s\n", res->ai_addr->sa_data);
		ptr = ptr->ai_next;
	}
}