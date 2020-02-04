#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char**argv) {
	struct addrinfo hints, *res, *ptr;
	int err = 0;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;		//Allow IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	//TCP socket

	//argv[1] is the website name example "www.google.com"
	if ((err = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE); 	
	}
	
	//res contains the linked list of all the addresses which we iterate
	for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
		//INET6_ADDRSTRLEN has enough space for IPv4 and IPv6
		char IP[INET6_ADDRSTRLEN];					
		void *addr;
		char *iptype;

		if (ptr->ai_family == AF_INET) {		//IPv4
			struct sockaddr_in *saddr;
			saddr = (struct sockaddr_in *)ptr->ai_addr;
			addr = &(saddr->sin_addr);
			iptype = "IP4";	
		} else {								//IPv6
			struct sockaddr_in6 *saddr6;
			saddr6 = (struct sockaddr_in6 *)ptr->ai_addr;
			addr = &(saddr6->sin6_addr);
			iptype = "IP6";
		}

		//convert ip addr to presentable format 
		if (inet_ntop(ptr->ai_family, addr, IP, INET6_ADDRSTRLEN) == NULL) {
			perror("inet_ntop");
			exit(EXIT_FAILURE);
		}
		printf("%s for %s is: %s\n", iptype, argv[1], IP);
	}
	freeaddrinfo(res);
	return 0;
}
