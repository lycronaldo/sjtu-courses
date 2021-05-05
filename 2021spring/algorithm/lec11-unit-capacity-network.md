## Simple Unit-Capacity Network

重点：Hall’s Marriage Theorem 可能会考。

本节课的内容包括：

- Bipartite Matching
- Hall’s Marriage Theorem
- Hackathon Problem
- Simple Unit-Capacity Networks



## Bipartite Matching

### Definitions

参考 `lec02.md` .

回顾一下匹配和最大匹配的概念：

- **Matching**: Given an undirected graph $G = (V,E)$, subset of edges $M \sube E$ is a matching if each node appears in at most one edge in $M$ (some node maybe not appear in $M$ ).
- **Maximum Matching**: Given a graph $G$, find a max-cardinality matching. Obviously, the "max-cardinality" is no more than $|V|/2$ .

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424162958.png" style="width:50%" />

二分匹配：

- **Bipartite Graph**: A graph $G$ is bipartite if the nodes can be partitioned into two subsets $L$ and $R$ such that every edge connects a node in $L$ with a node in $R$.
- **Bipartite Matching**: Given a bipartite graph $G = (L \cup R, E)$, find a max-cardinality matching.

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424163344.png" style="width:30%;" />

### Max-Flow Formualtion

- Direct all edges in $G$ from $L$ to $R$ (with capacity 1 or $\infin$, both are ok) .

- Add a node $s$, and an edge $(s,x)$ from $s$ to each node in $L$ (**must** have **unit-capacity**). 

- Add a node $t$, and an edge $(y,t)$ from each node in $R$ to $t$ (**must** have **unit-capacity**).

对于所有 $L \rightarrow R$ 的边，可以设置为 $\infin$ ，是为了方便后续的证明，这样的设置并不影响最大流与最大匹配的对应关系，因为每一个 Augmenting Path 的瓶颈容量被 $(s, x)$ 和 $(y, t)$ 限制为 1 。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424164617.png" style="width:60%;" />

### Proof of Correctness

**Theorem**: 1-1 correspondence between matchings of cardinality $k$ in $G$ and integral flows of value $k$ in $G'$.

> **Proof**
>
> - [ $\Rightarrow$ ]
>   - Let $M$ be a matching in $G$ of cardinality $k$.
>   - Consider flow $f$ that sends 1 unit on each of the $k$ corresponding paths.
>   - $f$ is a flow of value $k$.
> - [ $\Leftarrow$ ]
>   - Let $f$ be an integral flow in $G'$ of value $k$.
>   - Consider $M$ = set of edges from $L$ to $R$ with $f(e)=1$.
>     - each node in $L$ and $R$ participates in at most one edge in $M$.
>     - $|M| = k$: apply flow-value lemma to cut $(L \cup \{s\}, R \cup \{t\})$ . (See lemma of "Max-Flow Min-Cut Theorem" in `lec09.md`)



**Corollary**: We can solve bipartite matching problem via max-flow formulation.

> **Proof**
> - Integrality theorem $\Rightarrow$ there exists a max flow $f^*$ in $G'$ that is integral.
> - 1-1 correspondence $\Rightarrow$ $f^*$ corresponds to max-cardinality matching.

通过 Max-flow 来解决二分图的最大匹配，可以在 $O(VE)$ 时间内完成。



## Perfect Matching in Bigraphs

**Definition**

> Given a graph $G = (V,E)$, a subset of edges $M \sube E$ is a perfect matching if each node appears in exactly one edge in $M$.
>
> Q: When does a bipartite graph have a perfect matching? (Hall's Theorem)

**Notation**

- Let $S$ be a subset of nodes, and let $N(S)$ be the set of nodes adjacent to nodes in $S$ .
- If a bipartite graph $G = (L \cup R, E)$ has a perfect matching, then $|N(S)| \ge S$ for all subsets $S \sube L$ .
  - According to the definition of $N(S)$, each node in $S$ has to be connected with a different node in $N(S)$ .



**Hall's Theorem** 

Let $G = (L \cup R, E)$ be a bipartite graph with $|L| = |R|$. Then, graph $G$ has a perfect matching iff $|N(S)| \ge |S|$ for all subsets $S \sube L$.

> **Proof** [=>]
>
> - According to the definition of $N(S)$ , it is obvious.
>
> **Proof** [<=]
>
> - Suppose $G$ does not have a perfect matching.
> - Formulate as a max-flow problem and let $(A,B)$ be a min-cut in $G'$ (then we have the max-matching of $G$ ).
> - By Max-flow Min-Cut Theorem, $\text{cap}(A, B) < |L|$ .
>   - 我们假设 $G$ 不存在完美匹配，因此 $G'$ 的最大流（最小割）最多只能为 $|L|-1$ 。
> - Define $L_A = L \cap A, L_B = L \cap B, R_A = R \cap A$.
>   - Then $\text{cap}(A,B) = |L_B| + |R_A| \Rightarrow |R_A| < |L_A|$
>   - $\text{cap}(A,B)$ 定义为从 A 出发，到 B 结束的所有边。
> - Min-cut can use $\infin$ edges $\Rightarrow$ $N(L_A) \sube R_A$ .
>   - Then $|N(L_A)| \le |R_A| < |L_A|$ .
> - This is in contradition with $|N(S)| \ge S$ .
>
> 晕了，最后 2 步的推导没看懂 😅 。



## References

- [1] KT05 - Algorithm Design (Section 7.5)