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

TEST(FiniteStateMachine, IncarnationNumberOverridesForNonFailures) {
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

  std::string node_address = "localhost:8080";
  
  std::vector<Update> updates_2 {
    Update {
      Node { "localhost", 8080 },
      Status { NodeStatus::SUSPECTED, 2 }
    }
  };

  fsa.handleUpdates(updates_2);

  Status status = Status { NodeStatus::SUSPECTED, 2 };

  ASSERT_EQ(fsa.getNodeStatus(node_address), status);

  std::vector<Update> updates_3 {
    Update {
      Node { "localhost", 8080 },
      Status { NodeStatus::ALIVE, 1 }
    }
  };

  fsa.handleUpdates(updates_3);

  ASSERT_EQ(fsa.getNodeStatus(node_address), status);

  std::vector<Update> updates_4 {
    Update {
      Node { "localhost", 8080 },
      Status { NodeStatus::ALIVE, 3 }
    }
  };

  fsa.handleUpdates(updates_4);

  status = Status { NodeStatus::ALIVE, 3 };
  ASSERT_EQ(fsa.getNodeStatus(node_address), status);

  std::vector<Update> updates_5 {
    Update {
      Node { "localhost", 8080 },
      Status { NodeStatus::SUSPECTED, 2 }
    }
  };

  fsa.handleUpdates(updates_5);

  ASSERT_EQ(fsa.getNodeStatus(node_address), status);
}
