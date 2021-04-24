## Simple Unit-Capacity Network

本节课的内容包括：

- Bipartite Matching
- Hall’s Marriage Theorem
- Hackathon Problem
- Simple Unit-Capacity Networks



## Bipartite Matching

### Definitions

回顾一下匹配和最大匹配的概念：

- **Matching**: Given an undirected graph $G = (V,E)$, subset of edges $M \sube E$ is a matching if each node appears in at most one edge in $M$ (some node maybe not appear in $M$ ).
- **Maximum Matching**: Given a graph $G$, find a max-cardinality matching. Obviously, the "max-cardinality" is no more than $|V|/2$ .

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424162958.png" style="width:50%" />

二分匹配：

- **Bipartite Graph**: A graph $G$ is bipartite if the nodes can be partitioned into two subsets $L$ and $R$ such that every edge connects a node in $L$ with a node in $R$.
- **Bipartite Matching**: Given a bipartite graph $G = (L \cup R, E)$, find a max-cardinality matching.

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424163344.png" style="width:30%;" />

### Max-Flow Formualtion

Formulations:

- Direct all edges in $G$ from $L$ to $R$.
- Add a node $s$, and an edge $(s,x)$ from $s$ to each node in $L$. 
- Add a node $t$, and an edge $(y,t)$ from each node in $R$ to $t$.
- Give each edge in $G'$ a capacity of 1.

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424164617.png" style="width:60%;" />

### Proof of Correctness

**Theorem**: 1-1 correspondence between matchings of cardinality $k$ in $G$ and integral flows of value $k$ in $G'$.

- [ $\Rightarrow$ ]
  - Let $M$ be a matching in $G$ of cardinality $k$.
  - Consider flow $f$ that sends 1 unit on each of the $k$ corresponding paths.
  - $f$ is a flow of value $k$.
- [ $\Leftarrow$ ]
  - Let $f$ be an integral flow in $G'$ of value $k$.
  - Consider $M$ = set of edges from $L$ to $R$ with $f(e)=1$.
    - each node in $L$ and $R$ participates in at most one edge in $M$.
    - $|M| = k$: apply flow-value lemma to cut $(L \cup \{s\}, R \cup \{t\})$ . (See lemma of "Max-Flow Min-Cut Theorem" in `lec09.md`)



## References

- [1] KT05 - Algorithm Design (Section 7.5)