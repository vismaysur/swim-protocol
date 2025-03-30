#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include "fsa.hpp"
#include "messages.hpp"

#define BACKLOG 10
#define PORT    "3950"

void* get_in_addr(struct sockaddr* sa) {
  if (sa->sa_family == AF_INET) {
    return &(reinterpret_cast<struct sockaddr_in*>(sa)->sin_addr);
  }

  return &(reinterpret_cast<struct sockaddr_in6*>(sa)->sin6_addr);
}

int main() {
  int status {};
  struct addrinfo hints {};
  struct addrinfo *res = nullptr;
  int sockfd {};

  std::memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((status = getaddrinfo("", PORT, &hints, &res)) != 0) {
    std::cerr << "getaddrinfo(...): " << gai_strerror(status) << std::endl;
    return EXIT_FAILURE;
  }

  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
    freeaddrinfo(res);
    std::cerr << "socket(...): " << strerror(errno) << std::endl;
    return EXIT_FAILURE;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, res->ai_addr, res->ai_addrlen) == -1) {
    close(sockfd);
    freeaddrinfo(res);
    std::cerr << "setsockopt(...): " << strerror(errno) << std::endl;
    return EXIT_FAILURE;
  }

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    close(sockfd);
    freeaddrinfo(res);
    std::cerr << "setsockopt(...): " << strerror(errno) << std::endl;
    return EXIT_FAILURE;
  }

  freeaddrinfo(res);

  std::cout << "Server listening for incoming packets on port " << PORT << std::endl;

  struct sockaddr_storage client_addr {};
  socklen_t client_addr_len = sizeof(struct sockaddr_storage); 
  char client_addr_str[INET6_ADDRSTRLEN];

  while (1) {
    int numbytes {};
    char buf[1024];

    if ((numbytes = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr *) &client_addr, &client_addr_len)) == -1) {
      close(sockfd);
      std::cerr << "recvfrom(...): " << strerror(errno) << std::endl;
      return EXIT_FAILURE;
    }

    inet_ntop(client_addr.ss_family, 
        get_in_addr((struct sockaddr *)&client_addr),
        client_addr_str, sizeof(client_addr_str));

    std::cout << "Accepted packet from client " << client_addr_str << std::endl; 
  }

  close(sockfd);

  return EXIT_SUCCESS;
}
