## Scalable Causal Consistency for Wide-Area Storage with COPS

- Date: 2021/03/08
- Paper: [Don’t Settle for Eventual: Scalable Causal Consistency for Wide-Area Storage with COPS](https://ipads.se.sjtu.edu.cn/courses/ads/paper/cops-sosp2011.pdf)

**Question**

> Suppose an application client at data center $D1$ writes object $x$ with version 2 ($x_2$) and then object y with version 3 ($y_3$). Suppose $y_3$ has propagated from data center $D1$ to data center $D2$ but $x_2$ has not yet arrived at $D2$. Suppose another application client data center $D2$ has just read $y_3$, is it possible that it might read $x_1$ next? (If not, why not?) Will the client be blocked waiting for $x_2$ to arrive from $D1$? (If not, why not?)

**My Answer**



## Abstract

> Geo-replicated, distributed data stores that support complex online applications, such as social networks, must provide an “always-on” experience where operations always complete with low latency.
> Today’s systems often sacrifice strong consistency to achieve these goals, exposing inconsistencies to their clients and necessitating complex application logic. In this paper, we identify and define a consistency model—causal consistency with convergent conflict handling, or causal+—that is the strongest achieved under these constraints.

对于复杂的在线应用程序，其分布式数据存储必须是 Always Online 的，并且操作是 Low Latency 的。现在的系统会牺牲 Strong Consistency 来实现这些目标，但这样会把 *数据的不一致性* 暴露给用户，并且需要实现复杂的应用程序逻辑。

本文提出了一个一致性模型—— Causal Consistency，具有收敛冲突处理的因果一致性模型。或者说是 *Causal+* ，在某些限制条件下具有最强的一致性。

> We present the design and implementation of COPS, a key-value store that delivers this consistency model across the wide-area. A key contribution of COPS is its scalability, which can enforce causal dependencies between keys stored across an entire cluster, rather than a single server like previous systems. The central approach in COPS is tracking and explicitly checking whether causal dependencies between keys are satisfied in the local cluster before exposing writes. Further, in COPS-GT, we introduce get transactions in order to obtain a consistent view of multiple keys without locking or blocking. Our evaluation shows that COPS completes operations in less than a millisecond, provides throughput similar to previous systems when using one server per cluster, and scales well as we increase the number of servers in each cluster. It also shows that COPS-GT provides similar latency, throughput, and scaling to COPS for common workloads.

基于上述的一致性模型，设计与实现了 COPS 和 COPS-GT，可以怎么怎么样提高性能，但是存在什么什么样的缺点。



## 1. Introduction

分布式数据存储是现代 Web 服务的基本模块。CAP 定理已经证明：Consistency，Availability 和 Partition Torrence 三者中最多只能满足 2 个性质。现在的 Web 服务牺牲 Strong Consistency 来满足其余 2 个性质。

ALPS System: Availability, Low Latency, Partition Torrence and Scalability .

本文的目的是在满足 ALPS 这 4 个约束条件下，寻找最强的一致性模型——Casual Consistency with convergent conflict handling, 称之为 *Causal+ Consistency* 。

 *Causal+ Consistency* 可以保证若干数据操作之间的因果关系。

Main Contributions:

- explicitly identify four important properties of distributed data stores and use them to define ALPS systems
- name and formally define causal+ consistency (这都算 Contribution, 我人都 😵 了)
- present the design and implementation of COPS, a scalable system that efficiently realizes the causal+ consistency model.
-  present a non-blocking, lock-free get transaction algorithm in COPS-GT that provides clients with a consistent view of multiple keys in at most two rounds of local operations
- show through evaluation that COPS has low latency, high throughput, and scales well for all tested workloads; and that COPS-GT has similar properties for common workloads.

## 2. ALPS System and Trade-offs

ALPS 的 5 个 desirable properties:

- **A**vailability: 保证每个请求不管成功或者失败都有响应。
- **L**ow Latency
- **P**artition Torrence: 两个节点之间的连接丢失或暂时延迟，系统应当能够正常工作。
- High **S**calability: The data store scales out linearly. Adding $N$ resources to the system increases aggregate throughput and storage capacity by $O(N)$.
- Strict Consistency: 在分布式系统中的所有数据备份，在同一时刻是否同样的值。

在这 5 个期望属性中，必须要有 Trade-off 。

其中，ALP  属性可以实现 Always-Online 的特性。

## 3. Causal+ Consistency

假设数据都是 `<key, value>` 形式的数据，存在 `val = get(key)` 和 `put(key, val)` 这 2 种数据操作。

**Potential Causality**

1. Execution Thread. If $a$ and $b$ are two operations in a single thread of execution, then $a \leadsto b$ if operation $a$ happens before operation $b$.
2. Gets From. If $a$ is a `put` operation and $b$ is a `get` operation that returns the value written by $a$, then $a \leadsto b$.
3. Transitivity. For operations $a$ , $b$ , and $c$ , if $a \leadsto b$ and $b \leadsto c$ , then  $a \leadsto c$ .



### 3.1 Definition

通过 2 个属性来定义 *Causal+ Consistency*：

- *Causal consistency* requires that values returned from `get` operations at a replica are consistent with the order defined by $\leadsto$ (causality)
  - 直观的理解，对于某个数据 `x` 的操作，所有的 `put` 操作都必须要在 `get` 之前出现。
  - *Causal consistency* 就是说 `get`操作读到的值，和之前 $\leadsto$ 顺序的操作得到的结果，两者应当一致。
  - 冲突不受因果一致性的影响，使得数据副本永远存在差异。比如，`put(x, 1)`和 `put(x, 2)` 这 2 个操作，即使在存在因果一致性的约束条件，仍然允许一个副本返回 1 ，而另外一个副本返回 2 .
  - 因此需要 Causal+ 去处理冲突

- *Convergent conflict handling* requires that all conflicting puts be handled in the same manner at all replicas, using a handler function h. 
  - 所有冲突的 `put` 方法需要被一个函数 `h` 统一处理（「统一」意味着在所有的数据副本上，以相同的方式处理冲突）。
  - 函数 `h` 需要满足 associative and commutative （应该是交换律和结合律），即：`h(a, h(b, c))` 和 `h(c, h(b, a))` 等价。
  - 对于写冲突的处理，常用方法是 last-writer-wins rule ，只保留最后一次写的内容。
  - COPS 可自定义冲突函数，不过默认是last-writer-wins。



### 3.2 Causal+ vs. Other Consistency Models

常见的一致性模型（从强到弱）：

- linearizability (aka strictconsistency)
- sequential consistency
- causal consistency
- FIFO (PRAM) consistency
- per-key sequential consistency



### 3.3 Causal+ in COPS

在 COPS 系统中使用了两个抽象，即 version 和 dependencies ，以帮助我们推理 Causal+ Consistency .

- Version, 其实就是标记数据副本不同的值，例如 $key_1, key_2$ 表示 2 个不同版本的 $<key, val_1>, <key, val_2>$ ，Version 标识符满足：if $x_i \leadsto _i$ then $i < j$ .
  - 每个 key 有该字段。当 COPS 返回一个键 key，key.version = i，那么 Causal+ 确保之后的返回值，version一定等于或大于（因果序下）i
- Dependence：$a \leadsto b$ 即 $b$ 依赖于 $a$, COPS 为了保证 Causal+，只有在某个操作的依赖操作复制完成后，才会复制本操作，即如果 a, b 都是 `put` 操作，那么 b 的复制一定在 a 的复制之后



## 4. System Design of COPS







## Note

一致性模型，分为强一致性模型 (Strong) 和弱一致性模型 (Weak)  2 大类。

强一致性模型：

- 线性一致性 (Linearizable Consistency), aka Strict Consistency, Atomic Consistency.
- 顺序一致性 (Sequential Consistency)

弱一致性模型：

- 因果一致性 (Causal Consistency)
- 最终一致性 (Eventual Consistency)
- 以客户端为中心的一致性 (Client-centric Consistency)



