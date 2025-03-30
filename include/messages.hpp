#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class MessageType {
  PING = 1,
  PING_REQ = 2, 
  ACK = 3 
};

enum class NodeStatus {
  ALIVE = 1,
  FAILED = 2,
  SUSPECTED = 3
};

struct Node {
  std::string ip;
  uint32_t    port;
};

struct Status {
  NodeStatus  status;
  uint32_t    incarnation;

  bool operator==(const Status& other) const {
    return (this->status == other.status &&
    this->incarnation == other.incarnation);
  }
};

struct Update {
  Node    node;
  Status  status;
};

struct Message {
  std::vector<Update>  updates;
  MessageType   type;
};
