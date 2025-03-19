#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BACKLOG 10
#define PORT    "3950"

void* get_in_addr(struct sockaddr* sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main() {
  int status;
  struct addrinfo hints;
  struct addrinfo *res;
  int sockfd;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("", PORT, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo(...): %s\n", gai_strerror(status));
    exit(1);
  }

  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
    freeaddrinfo(res);
    fprintf(stderr, "socket(...): %s\n", strerror(errno));
    exit(1);
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, res->ai_addr, res->ai_addrlen) == -1) {
    close(sockfd);
    freeaddrinfo(res);
    fprintf(stderr, "setsockopt(...): %s\n", strerror(errno));
    exit(1);
  }

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    close(sockfd);
    freeaddrinfo(res);
    fprintf(stderr, "bind(...): %s\n", strerror(errno));
    exit(1);
  }

  freeaddrinfo(res);

  if (listen(sockfd, BACKLOG) == -1) {
    close(sockfd);
    freeaddrinfo(res);
    fprintf(stderr, "listen(...): %s\n", strerror(errno));
    exit(1);
  }

  printf("Server listening for incoming connections on port %s!\n", PORT);

  struct sockaddr_storage client_addr;
  socklen_t client_addr_len = sizeof(struct sockaddr_storage); 
  char client_addr_str[INET6_ADDRSTRLEN];

  while (1) {
    int conn;
    if ((conn = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_len)) == -1) {
      close(sockfd);
      fprintf(stderr, "accept(...): %s\n", strerror(errno));
      exit(1);
    }

    inet_ntop(client_addr.ss_family, 
        get_in_addr((struct sockaddr *)&client_addr),
        client_addr_str, sizeof(client_addr_str));

    printf("Accepted connection from client (%s)\n", client_addr_str);
  }

  close(sockfd);

	return 0;
}
