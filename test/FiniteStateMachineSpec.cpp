#include "fsa.hpp"
#include "messages.hpp"

#include <vector>
#include <gtest/gtest.h>

TEST(FiniteStateMachine, InsertsNewNodes) {
  FiniteStateMachine fsa {};

  std::vector<Update> updates {
    Update {
        Node { "localhost", 8080 },
        Status { NodeStatus::ALIVE, 1 }
    },
    Update {
        Node { "localhost", 8081 },
        Status { NodeStatus::ALIVE, 1 }
    },
    Update {
        Node { "localhost", 8082 },
        Status { NodeStatus::ALIVE, 1 }
    } 
  };

  fsa.handleUpdates(updates);

  ASSERT_TRUE(fsa.getGroupSize() == 3);
}
