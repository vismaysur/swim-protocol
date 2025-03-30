#include "messages.hpp"
#include "fsa.hpp"

#include <unordered_map>
#include <cstdlib>
#include <vector>

int FiniteStateMachine::handleUpdates(std::vector<Update> updates) {
  for (int i = 0; i < updates.size(); i++) {
    Update update = updates[i];
    Node node = update.node;
    Status status = update.status;

    std::string address = node.ip + ":" + std::to_string(node.port);

    if (group.find(address) == group.end() &&
        status.status == NodeStatus::ALIVE) {
        group.insert({address, status});
    } else {
        Status curr_status = group[address];

        if (curr_status.status == NodeStatus::FAILED) continue;

        if (status.status == NodeStatus::FAILED) {
          group[address] = status;
        }

        if (status.incarnation > curr_status.incarnation) {
          group[address] = status;
        }
    }
  }

  return EXIT_SUCCESS;
}

int FiniteStateMachine::getGroupSize() {
  return group.size();
}

Status FiniteStateMachine::getNodeStatus(std::string address) {
  return group.at(address); 
}
