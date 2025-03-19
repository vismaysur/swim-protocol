#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void* get_in_addr(struct sockaddr* sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr); 
}

int main() {
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status; 

	if ((status = getaddrinfo(NULL, "3950", &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo(...): %s\n", gai_strerror(status));
		exit(1);
	}

	int sockfd;

	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		fprintf(stderr, "socket(...): %s\n", strerror(errno)); 
		exit(1);
	}

	int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		close(sockfd);
		fprintf(stderr, "setsockopt(...): %s\n", strerror(errno));
		exit(1);
	}

	if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		close(sockfd);
		fprintf(stderr, "bind(...): %s\n", strerror(errno));
		exit(1);
	}

	if (listen(sockfd, 10) == -1) {
		close(sockfd);
		fprintf(stderr, "listen(...): %s\n", strerror(errno));
		exit(1);
	}

	printf("server listening for connections...\n");

	struct sockaddr_storage client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

	char client_str[INET6_ADDRSTRLEN];

	while (1) {
		int conn;

		if ((conn = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
			close(sockfd);
			fprintf(stderr, "accept(...): %s\n", strerror(errno));
			exit(1);
		}

		inet_ntop(client_addr.ss_family,
				get_in_addr((struct sockaddr *) &client_addr),
				client_str, sizeof(client_str));

		printf("Accepted connection from client (%s)\n", client_str);
	}

	freeaddrinfo(res);

	return 0;
}
