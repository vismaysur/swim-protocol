# swim-protocol

The SWIM protocol is a weakly consistent group membership protocol, known (within the dist sys research community) for being the first protocol wherein the expected time to first detection of process failure and message load per member does not depend on the group size. It seperates the failure detection and knowledge disemmination components; at the same time, group-wide membership changes are piggy backed on inter-node ping/ack messages. This results in a epidemic / infection style of disemmination (which guarantees the latency and message load properties mentioned above).

The [paper](https://www.cs.cornell.edu/projects/Quicksilver/public_pdfs/SWIM.pdf) was published by Abhinandan Das, Indranil Gupta, Ashish Motivala at Cornell University.

## Why i'm building this project

Several reliable and high quality implementations of group membership protocols have been built over the years; we definitely don't need another.

This project is my attempt at growing as an engineer across these 2 areas:
1) A first-principles understanding of classical distributed systems
2) Modern C++ with a focus on memory safety and high-performance 
