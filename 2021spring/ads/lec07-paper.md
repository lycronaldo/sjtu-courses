## Paxos Made Simple

- Date: 2021/4/12
- Paper: [Paxos Made Simple](http://lamport.azurewebsites.net/pubs/paxos-simple.pdf)

一篇 Paper Reading Note。

## Preparation

**拜占庭将军问题**

拜占庭将军问题是由 Leslie Lamport 在论文  *[The Byzantine Generals Problem](https://www.microsoft.com/en-us/research/uploads/prod/2016/12/The-Byzantine-Generals-Problem.pdf)* 中提出的：

> 一组拜占庭将军分别各带领一支军队共同进攻一座城市，每支军队的行动策略限定为**进攻或撤退**两种。
>
> 为了保证军队的成功（无论是进攻还是撤退），各位将军必须通过投票来达成一致策略，即所有军队一起进攻或所有军队一起撤离。因为各位将军分处目标城市不同方位，他们只能通过信使互相联系。在投票过程中每位将军都将自己投票给进攻还是撤退的信息通过信使分别通知其他所有将军，这样一来每位将军根据自己的投票和其他所有将军送来的信息就可以知道共同的投票结果而决定行动策略。

在上述场景下，可能存在这样的问题：

- 将军中可能会出现叛徒，叛徒会对「糟糕的策略」投票，还可能选择性投票。
  - 假设有 2n+1 个将军，1 个是叛徒，这个叛徒对 n 个发送进攻信号（这 n 个也赞成进攻），对另外 n 个发送撤退信号（这 n 个也赞成撤退）。这样，在 n 个赞成进攻的将军看来，得到的结果是 n+1 进攻，n 撤退，所以选择进攻；在 n 个赞成撤退的将军看来，n+1 撤退，n 进攻，所以选择撤退。因此，军队的策略无法达成共识。
- 信号传递需要信使。信使可能丢失或者篡改信件，信使本身也有可能丢了（如被敌军斩杀），这也会影响投票的结果。

在分布式系统中，每一个服务器节点就是一个将军，节点之间的网络服务就是信使。实际上，这是在描述计算机中的点对点通信的问题：基于不可靠信道（意味着存在消息丢失或篡改的可能），通过消息传递的方式达成共识是不可能的。

一般而言，把服务器节点可能故障，但通信消息不会被篡改的情况称为非拜占庭模型 (Non-Byzantine Model) 。与之相对的，上面所述的场景就是拜占庭模型。

处理非拜占庭模型的算法主要有 Paxos 和 Raft ；处理拜占庭模型的算法主要有 PBFT (Practical Byzantine Fault Tolerance and Proactive Recovery) .



## Introduction

Paxos 是一个分布式系统中的共识 (Consensus) 算法，何谓 Consensus 呢？根据参考文献 [2] :

> Suppose you have a collection of computers and want them all to agree on something. This is what consensus is about; consensus means agreement.
>
> Consensus comes up frequently in distributed systems design. There are various reasons why we want it: to agree on who gets access to a resource (mutual exclusion), agree on who is in charge (elections), or to agree on a common ordering of events among a collection of computers (e.g., what action to take next, or state machine replication).

个人理解，由于分布式系统中数据副本 (Data Replicarion) 的存在，当同一数据的不同副本出现不同的值时，Consensus 需要对这些不同的值进行决策，确保一个服务器集群，对外总是对该数据的值保持一致。

Paxos 给我的感觉更像是一种协议 (Protocol) ，在服务器集群中实现这一协议，可以保证系统的某些性质（论文中分为 Safety 和 Liveness 两类）。与 TCP 类似，Paxos 的每个步骤都是为了解决某个问题而存在的，这也是这篇论文重点阐述的内容。在 Paxos 最原始的论文 [The Part-Time Parliament](https://lamport.azurewebsites.net/pubs/lamport-paxos.pdf) 中，其实就是把 Paxos 称作一个 Protocol 的。

Paxos 算法中存在 3 个不同的角色：

- Proposer: 可以给 Acceptor 提出一个 Value  。
- Acceptor: 接受来自 Proposer 提出的 Value，并决定是否选定这一 Value; 一旦某个 Value 在系统中被选定，告知其他 Learner 。
- Learner: 接受 Acceptor 告知的 Value ，并记录这个 Value 。

其中，Value 可以是由 Client 端发送给 Proposer 的。

需要注意的是，并不是一个服务器只能有一个角色，某个服务器节点允许同时为 Proposer, Acceptor 和 Learner 。一般场景下，Acceptor  集合和 Learner 集合都是整个服务器集群，Proposer 集合可以是某几个负责写操作的服务器。

此外，Paxos 是基于系统为非拜占庭模型的前提下实现的，它的基本假设如下（参考 Refs [2] ）：

> - **Concurrent proposals**: One or more systems may propose a value concurrently. If only one system would propose a value then it is clear what the consensus would be. With multiple systems, we need to select one from among those values.
> - **Validity**: The chosen value that is agreed upon must be one of the proposed values. The servers cannot just choose a random number.
> - **Majority rule**: Once a majority of Paxos servers agrees on one of the proposed values, we have consensus on that value. This also implies that a majority of servers need to be functioning for the algorithm to run. **To survive $m$ failures, we will need $2m+1$ systems.**
> - **Asynchronous network**: The network is **unreliable** and asynchronous: messages may get **lost** or arbitrarily delayed. The network may also get partitioned.
> - Fail-stop faults: Systems may exhibit fail-stop faults. They may restart but need to remember their previous state to make sure they do not change their mind. Failures are not Byzantine.
> - Unicasts: Communication is **point-to-point**. There is no mechanism to multicast a message atomically to the set of Paxos servers.
> - Announcement: Once consensus is reached, the results can be made known to everyone.

## Paxos Algorithm

### Choosing a Value

根据论文所述，Paxos Choosing 算法可分为 2 步。

**Phase 1**

- (a) A proposer selects a proposal number $n$ and sends a prepare request with number $n$ to a majority of acceptors.
- (b) If an acceptor receives a prepare request with number $n$ greater than that of any prepare request to which it has already responded, then it responds to the request with a promise not to accept any more proposals numbered less than $n$ and with the highest-numbered proposal (if any) that it has accepted.

**Phase 2**

- (a) If the proposer receives a response to its prepare requests (numbered $n$) from a majority of acceptors, then it sends an accept request to each of those acceptors for a proposal numbered $n$ with a value $v$, where $v$ is the value of the highest-numbered proposal among the responses, or is any value if the responses reported no proposals.
- (b) If an acceptor receives an accept request for a proposal numbered $n$, it accepts the proposal unless it has already responded to a prepare request having a number greater than $n$.

"a majority" 在论文中没有给出量化的说明，个人理解是严格大于一半就可称为 "a majority" ，而在实际具体的 Paxos 实现当中，可以是 2/3 或者 3/4 等数值。

上面 2 个阶段分别出现了 2 中请求类型，分别为 Prepare Request 和 Accept Request，下面分别记为 $Preq$ 和 $Areq$ 。

算法描述（图出自 Refs [3] ）：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210412101536.png" style="width:80%;" />

更加详细的描述可以参考 Appendix 一节。



### Learning a Value

Learner 获取被选定的 Value 的方式可以有 2 种：

- L1: Each acceptor, whenever it accepts a proposal, respond to all learners, sending them the proposal.
- L2: Acceptors respond with their acceptances to a distinguished learner, which in turn informs the other learners when a value has been chosen.
  - It is also less reliable, since the distinguished learner could fail.
  - L3: More generally, the acceptors could respond with their acceptances to some set of distinguished learners, each of which can then inform all the learners when a value has been chosen.

**Summary**

假设有 $m$ 个 Acceptor，$n$ 个 Learner 。

| Methods |                         Description                          |                         Feature                          |
| :-----: | :----------------------------------------------------------: | :------------------------------------------------------: |
|   L1    | 每个 Acceptor 一旦接受了某一个 Proposal，就把 Proposal 的 Value 发送给所有的 Learner |    Learner 能最快获取选定的 Value；通信次数为 $mn$ .     |
|   L2    | 在 Learner 中选取一个主 Learner (Distinguished Learner)，Acceptor 把 Value 发送给主 Learner，再由它发送给其他的 Learner |      主 Learner 节点可能故障；通信次数为 $m+n-1$ .       |
|   L3    | 选取若干个主 Learner ，Acceptor 把 Value 发送给这个主 Learner 集合，这个集合里面的每个主 Learner 再转发给它负责的 Learner 集合 | 是 L2 的泛化形式，具有更好的可靠性，但通信网络复杂度更高 |

由于信息丢失的可能性，当某一个 Acceptor 故障时，那么它负责的 Learner 的集合中的 Learner 可能不知道系统已经选定了某一个 Value 。在这种情况下，这些 Learner 可以作为 Proposer 提出一个 Proposal ，那么从 Acceptor 给出的响应中能获取到已选定的 Value 。



### Progress

论文的第 2.4 小节，说明了一个不满足 Liveness 性质的场景，并提出改进措施。

根据上述的算法，可能出现这么一种场景：存在 2 个 Proposer $P_1$ 和 $P_2$，不断地**交替**向 Acceptor 发送 Prepare 请求，那么 Acceptor 中的 `keepN` 一直增大：

1. $P_1$ 发送 $Preq(n_1)$ ，Acceptor 接受并把 `keepN` 更新为 $n_1$ 。
2. $P_2$ 发送 $Preq(n_2), n_2 > n_1$ ，Acceptor 接受并把 `keepN` 更新为 $n_2$ 。
3. $P_1$ 发送 $Areq(n_1, v_1)$ ，那么 Acceptor 会拒绝，$P_1$ 再次发送 $Preq(n_3), n_3 > n_2$ ，Acceptor 接受并更新 `keepN` 为 $n_3$ 。
4. $P_2$ 发送 $Areq(n_2, v_2)$ ，那么 Acceptor 会拒绝，$P_2$ 再次发送 $Preq(n_4), n_4 > n_3$ ，Acceptor 接受并更新 `keepN` 为 $n_4$ 。

这样的行为不断交替，会形成类似于死锁的状态，最终没有一个 Value 会被选中。

改进办法：

- 一旦 Proposer 的某个 $Preq$ 被拒绝，加入一个随机延迟 $t$，经过时间 $t$ 后它才能继续发送 $Preq$ 。
- 在任意时刻，只允许有一个 Proposer ，称为 Distinguished Proposer 或者 Leader （Raft 中的叫法）。
  - 如果 Leader 故障，那么重新选取一个。
  - 经过特定的时间，可以考虑换一个 Leader 。



## Summary

粗略读了一遍 Paxos Made Simple 这篇论文，推导过程有不少地方没怎么看明白，最后是结合网上的 Blog 直接看最后的算法模型。

但其实还有很多内容没有提及到，比如这里阐述的是最原始的 Paxos，称为 Basic Paxos，还有一种称为 Multi Paxos 的高级形式。



## Appendix

参（照）考（抄） Refs [2] 的伪代码。

在 Proposer 中维护 `<ID, cnt>` 2 个变量，在 Acceptor 中维护 `<max_id, accepted_id, accepted_value>` 3 个变量。

那么 Choosing a Value 的 2 个阶段可以用下面的伪代码来描述。

**Phase 1-a: Proposer (PREPARE)**

A proposer initiates a PREPARE message, picking a unique, ever-incrementing value.

```text
ID = cnt++;
send PREPARE(ID)
```

**Phase 1-b: Acceptor (PROMISE)**

An acceptor receives a `PREPARE(ID)` message:

```text
if (ID <= max_id)
    do not respond (or respond with a "fail" message)
else
    max_id = ID     // save highest ID we've seen so far
    if (proposal_accepted == true) // was a proposal already accepted?
        respond: PROMISE(ID, accepted_ID, accepted_VALUE)
    else
        respond: PROMISE(ID)
```

**Phase 2-a: Proposer (PROPOSE)**

The proposer now checks to see if it can use its proposal or if it has to use the highest-numbered one it received from among all responses:

```text
did I receive PROMISE responses from a majority of acceptors?
if yes
    do any responses contain accepted values (from other proposals)?
    if yes
        val = accepted_VALUE    // value from PROMISE message with the highest accepted ID
    if no
        val = VALUE     // we can use our proposed value
    send PROPOSE(ID, val) to at least a majority of acceptors
```

**Phase 2-b: Acceptor (ACCEPT)**

Each acceptor receives a `PROPOSE(ID, VALUE)` message from a proposer. If the `ID` is the highest number it has processed then accept the proposal and propagate the value to the proposer and to all the learners.

```
if (ID >= max_id) // is the ID the largest I have seen so far?
    proposal_accepted = true     // note that we accepted a proposal
    accepted_ID = ID             // save the accepted proposal number
    accepted_VALUE = VALUE       // save the accepted proposal data
    respond: ACCEPTED(ID, VALUE) to the proposer and all learners
else
    do not respond (or respond with a "fail" message)
```

If a majority of acceptors accept `<ID, value>` then consensus is reached. Consensus is on the `value`, not necessarily the ID.

Refs [4] 的一个图解，跟上面的伪代码「无缝衔接」，个人觉得这比上面 Refs [3] 的图解更容易理解。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210412170301.jpeg" style="width:90%;" />



##  References

- [1] [Paxos Made Simple](http://lamport.azurewebsites.net/pubs/paxos-simple.pdf)
- **[2] https://www.cs.rutgers.edu/~pxk/417/notes/paxos.html**
- **[3] https://www.cs.utah.edu/~stutsman/cs6450/public/paxos.pdf**
- **[4] https://www.cnblogs.com/linbingdong/p/6253479.html**
- [5] [Paxos Visualization](http://www.scs.stanford.edu/20sp-cs244b/projects/Paxos%20Visualization.pdf)

加粗的 Item 为推荐阅读文章。



## Submission

每节课要求提交的小作业，回答基于论文内容的一个问题，个人认为有助于理解 Paxos 。

**Synopsis**

This paper describes the Paxos consensus algorithm. In chapter 2, it mainly introduces how to choose and learn a value among proposers, acceptors and learners. In chapter 3, it mainly describes how to implement a deterministic state machine in the server(s) cluster.

**Question**

> Suppose that the acceptors are A, B, and C. A and B are also proposers. How does Paxos ensure that the following sequence of events can't happen? What actually happens, and which value is ultimately chosen? 
>
> - A sends prepare requests with proposal number 1, and gets responses from A, B, and C.
> - A sends accept(1, "foo") to A and C and gets responses from both. Because a majority accepted, A thinks that "foo" has been chosen. However, A crashes before sending an accept to B. 
> - B sends prepare messages with proposal number 2, and gets responses from B and C.
> - B sends accept(2, "bar") messages to B and C and gets responses from both, so B thinks that "bar" has been chosen.

**My Answer**

The last events can't happen, because Paxos have the rule of only allowing committing previously accepted values. Since C have accepted "foo" as proposal, it will reply to B's prepare request with value "foo". This ensures that B will choose "foo" as its proposal value, which implies B will send `accept(2, "foo")` message to B and C. Therefore, "foo" would be the choosen value.
