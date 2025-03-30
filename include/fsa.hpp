#pragma once

#include <unordered_map>
#include <vector>
#include "messages.hpp"

class FiniteStateMachine {
  private:
    // The key is a string type
    // The key is a node's ip address + port
    std::unordered_map<std::string, Status> group {};

  public:
    int handleUpdates(std::vector<Update> updates);

    int getGroupSize();

    Status getNodeStatus(std::string address);
};
