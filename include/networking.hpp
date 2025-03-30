#pragma once

class Networking {
  public: 
    // Server and client functions must be run on seperate threads to be non-blocking.
    // This will change soon once epoll is added to the server implementation.

    // Loops and listens for incoming packets
    int runServer();

    // Handles PING, ACK AND PING-REQ logic for inter-node failure detection.
    int runClient();
};
