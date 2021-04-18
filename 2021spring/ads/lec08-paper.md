## Chubby

- Date: 2021/04/19
- Paper: [The Chubby lock service for loosely-coupled distributed systems](https://www.usenix.org/legacy/event/osdi06/tech/full_papers/burrows/burrows.pdf)

Paper Structure:

- 1 Introduction
- 2 Design
  - 2.1 Rationale
  - 2.2 System structure
  - 2.3 Files, directories and handles
  - 2.4 Locks and sequencers
  - 2.5 Events
  - 2.6 API
  - 2.7 Caching
  - 2.8 Sessions and KeepAlives
  - 2.9 Fail-overs
  - 2.10 Database implementation
  - 2.11 Backup
  - 2.12 Mirroring
- 3 Mechanism for scaling
  - 3.1 Proxies
  - 3.2 Partitioning
- 4 Use, surprise and design errors
  - 4.1 Use and behaviour
  - 4.2 Java clients
  - 4.3 Use as a name service
  - 4.4 Problems with fail-over
  - 4.5 Abusive clients
  - 4.6 Lessons learned
- 5 Comparison with related work
- 6 Summary
- 7 Acknowledments





## Submission

**Synopsis**

This paper describes the design and implementation of Chubby, a lock service for loosely-coupled distributed systems, in which:

- availablility and reliability (rather than high performance) are emphasized, 
- Paxos protocol is used to solve asynchronous consensus, and
- a central lock service is built to meet the specific needs of most developers.



**Question**

> Please compare the difference between consistent client caching and time-based caching.

**My Answer**

Time-based caching is suitable for caching frequently modified data. Since these data will fail and become invalid in a short time, time-based caching can help to reduce the invalid requests.

Since the lock holder is valid in a long time (compared to time-based caching), the data will be not changed during the time, and consistent client cahcing can help to reduce pressure on the DNS server.

