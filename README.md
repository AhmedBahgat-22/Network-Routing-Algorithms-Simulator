# Routing Algorithm Performance Simulator Using C++

**Author:** Ahmed Bahgat (Ahmed Abdelmoula)  
**Student ID:** 220303923  
**Department:** Computer Engineering, Istanbul Arel University

## Project overview

This educational C++ project explores routing strategies in computer networks. The accompanying [Project Proposal](Project_Proposal.pdf) describes a planned modular performance simulator comparing **Flooding**, **Shortest Path First (Dijkstra)**, **Distance Vector (Bellman–Ford)**, and **Link State** on the same weighted, flat network topology.

The proposal aims to measure hop count, path cost, control-message overhead, and convergence steps, and to produce a comparative results table. These are **proposed objectives**, not claims that every metric or comparison is implemented in the current code.

## Current implementation

- C++ routing algorithm classes and network graph abstractions for educational simulation.
- A separate Windows TCP image/file transfer demonstration using Winsock2, with three applications: `SenderNode`, `RoutingSimulator`, and `ReceiverNode`.
- Targeted fixes to socket send/receive reliability documented in [Review Notes](REVIEW_NOTES.md).

**Implementation boundary:** The TCP demo currently uses a fixed localhost Sender → Router → Receiver connection. It does **not** dynamically route real TCP connections through a Dijkstra-selected path. The proposed end-to-end metric comparison and common algorithm interface should be validated against the implementation before being described as complete.

## Documents

| Document | Description |
| --- | --- |
| [Project Proposal (PDF)](Project_Proposal.pdf) | Original academic proposal: objectives, architecture, UML, flowchart, implementation plan, and references. |
| [Project Report (PDF)](Project_Report.pdf) | Existing project report supplied with the source. |
| [UML Diagram](UML_Diagram.png) | UML image supplied with the source. |
| [Review Notes](REVIEW_NOTES.md) | Code-review findings, fixes, limitations, and testing guidance. |

## Proposed architecture

As described in the proposal:

1. **Topology Module** — models routers, weighted links, and a flat network graph.
2. **Routing Algorithms Module** — implements Flooding, Dijkstra, Distance Vector, and Link State.
3. **Simulation Engine** — runs experiments and records routing performance metrics.

The proposal's diagrams appear on pages 4–5 of the PDF.

## Build and run (Windows)

**Requirements:** Windows, Visual Studio with *Desktop development with C++*, Windows SDK, MSVC, and Winsock2 (`ws2_32.lib`). C++17 is recommended.

1. Open `RoutingSimulator/RoutingSimulator.slnx` in Visual Studio.
2. Build the solution.
3. Start `ReceiverNode` (listens on port 9000).
4. Start `RoutingSimulator` (listens on port 8000 and forwards to ReceiverNode).
5. Start `SenderNode`. Make sure its working directory contains `test.jpg`.
6. Check the received file against the original using a file hash.

**Testing status:** A successful Visual Studio build and end-to-end Windows transfer have not yet been independently verified in this package. See [Review Notes](REVIEW_NOTES.md) for remaining work.

## Project status and next steps

- Build and run all three applications on Windows.
- Validate file integrity and socket error handling.
- Test the routing algorithm implementations on known topologies.
- Integrate a common algorithm interface and collect comparable metrics if required by the proposal.
- Add reproducible test results and screenshots once validated.

## References

The original proposal cites Tanenbaum & Wetherall, Forouzan, Bellman, Dijkstra, and RFC 2328 (OSPF). See [Project Proposal](Project_Proposal.pdf) for the full bibliography.
