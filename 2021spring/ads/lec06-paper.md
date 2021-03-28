## Sinfonia: A New Paradigm for Building Scalable Distributed Systems

- Date: 2021/03/29
- Paper: [Sinfonia: A New Paradigm for Building Scalable Distributed Systems](https://www.cs.princeton.edu/courses/archive/fall08/cos597B/papers/sinfonia.pdf)

**Outline**
- [Abstract](#abstract)
- [1 Introduction](#1-introduction)
- [2 Assumptions and Goals](#2-assumptions-and-goals)
- [3 Design](#3-design)
    - [3.1 Principles](#31-principles)
    - [3.2 Basic components](#32-basic-components)
    - [3.3 Mini-transaction](#33-mini-transaction)
    - [3.4 Caching and consistency](#34-caching-and-consistency)
    - [3.5 Fault tolerance](#35-fault-tolerance)
    - [3.6 Other design considerations](#36-other-design-considerations)
- [4 Implementation And Algorithms](#4-implementation-and-algorithms)
    - [4.1 Basic Architecture](#41-basic-architecture)
    - [4.2 Mini-transaction protocol overview](#42-mini-transaction-protocol-overview)
    - [4.3 Mini-transaction protocol details](#43-mini-transaction-protocol-details)
    - [4.4 Recovery from coordinator crashes](#44-recovery-from-coordinator-crashes)
    - [4.5 Recovery from participant crashes](#45-recovery-from-participant-crashes)
    - [4.6 Recovery from crash of the whole system](#46-recovery-from-crash-of-the-whole-system)
    - [4.7 Log garbage collection](#47-log-garbage-collection)
    - [4.8  Further optimizations](#48--further-optimizations)
    - [4.9 Consistent backups](#49-consistent-backups)
    - [4.10 Replication](#410-replication)
    - [4.11 Configuration](#411-configuration)
- [5 Application: Cluster File System](#5-application-cluster-file-system)
    - [5.1 Data layout](#51-data-layout)
    - [5.2 Making modifications and caching](#52-making-modifications-and-caching)
    - [5.3 Node locality](#53-node-locality)
    - [5.4 Contention and load balancing](#54-contention-and-load-balancing)
- [6 Application: Group Communication](#6-application-group-communication)
    - [6.1 Basic design](#61-basic-design)
    - [6.2 Garbage collection](#62-garbage-collection)
    - [6.3 Optimizations](#63-optimizations)
- [7 Evaluation](#7-evaluation)
- [8 Relative Work](#8-relative-work)
- [9 Conclusion](#9-conclusion)



**Question**

> What's the difference between coordinator in mini-transaction's 2PC protocol and standard 2PC protocol?

**Submission**

This paper describes Sinfonia, a new paradigm to build scalable distributed systems, in which just need to manipulate data structures instead of handling with message-passing protocols. Sinfonia presents two key components, a memory-centric system architecture (designed to replace messaging-passing protocols) and the 'mini-transaction' operation which provides ACID properties to replicated data.

Based on Sinfonia, this paper designed two applications, a cluster file system (FS) and a group communication service (CS), which shows the advantages of Sinfonia compared with the traditional paradigm.

The major result of their implementation is the reduction in effort and code size necessary to implement user services compared  to their traditional counterparts.

The answer of above question is in section 3.3.

> Roughly speaking, a coordinator executes a transaction by asking participants to perform one or more transaction actions, such as retrieving or modifying data items. At the end of the transaction, the coordinator executes two-phase commit. In the first phase, the coordinator asks all participants if they are ready to commit. If they all vote yes, in the second phase the coordinator tells them to commit; otherwise the coordinator tells them to abort. In Sinfonia, coordinators are application nodes and participants are memory nodes.





