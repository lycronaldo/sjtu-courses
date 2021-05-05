## Max Flow Problems: Applications

本节的主要内容是介绍 Max Flow 的主要应用场景（即把其他问题规约到 Max Flow 问题来解决）：

- Edge-Disjoint Paths
- Network Connectivity (Menger’s Theorem)
- Disjoint Paths in Undirected Graphs
- Multiple Sources and Sinks in Max-flow
- Circulation with Supplies and Demands
- Survey Desgin
- Airline Scheduling
- Project Selection





## Edge-Disjoint Paths

**Definition**

> - **Edge-disjoint** - Two paths are edge-disjoint if they have no edge in common.
> - **Edge-disjoint paths problem** - Given a diagraph $G=(V,E)$ and two vertices $s$ and $t$, find the max number of edge-disjoint $s \rightarrow t$ paths.
> - Max-flow formulation - Assign **unit capacity** to every edge.

**Theorem** 1-1 correspondence between $k$ edge-disjoint $s \rightarrow t$ paths in $G$ and integral flows of value $k$ in $G'$.

> **Proof** $\Rightarrow$
>
> - Let $P_1, \dots, P_k$ be $k$ edge-disjoint $s \rightarrow t$ paths in $G$ .
> - Set $f(e) = 1$ if edge $e$ participates in some paths $P_j$ (at least one path) , otherwise 0 .
> - Since paths are disjoint, $f$ is a flow of value $k$ (but not max-flow yet) .
>
> **Proof** $\Leftarrow$
>
> - Let $f$ be an integral flow in $G'$ with value $k$.
> - Consider edge $(s, u)$ with $f(s, u) = 1$ . 
>   - by flow conservation, there exists an edge $(u, v)$ with $f(u, v) = 1$ . 
>   - continue until reach $t$, always choosing a new edge.
>   - 大白话：对于 $f(e) = 1$ 这样的边，总是存在后继边 $e'$，且 $f(e')=1$ ，直到路径延伸到汇点 $t$ .
> - Produces $k$ (not necessarily simple) edge-disjoint paths.
>   - 由于每个边的容量都是 1 ，因此能够保证存在 $k$ 个上述的路径。

**Corollary** We can solve edge-disjoint paths problem via max-flow formulation.

> **Proof**
>
> - Integrality theorem $\Rightarrow$ there exists a max-flow $f^*$ in $G'$ that is integral.
> - 1-1 correspondence $\Rightarrow$ $f^*$ corresponds to max number of edge-disjoint $s \rightarrow t$ paths in $G$.



## Network Connectivity

**Definition**

> - **Dis-connectivity** - A set of edges $F \sube E$ disconnects $t$ from $s$ if every $s \rightarrow t$ path uses at least one edge in $F$.
> - **Network connectivity** - Given a digraph $G = (V,E)$ and two nodes $s$ and $t$, find minimal number of edges whose removal disconnects $t$ from $s$.

我感觉，这个 $F$ 集合就是有向图的最小割集（之一）。



**Menger's Theorem**

> The max number of edge-disjoint $s \rightarrow t$ paths equals the min number of edges whose removal disconnects $t$ from $s$.
>
> **Proof** $\le$ 
>
> - Suppose the removal of $F \sube E$ disconnects $t$ from $s$, and $|F| = k$.
> - Every $s \rightarrow t$ path use at least one edge in $F$ (according to its definition) .
> - Hence the number of edge-disjoint paths is $\le k$ .
>
> **Proof** $\ge$
>
> - Suppose max number of edge-disjoint $s \rightarrow t$ paths is $k$.
> - Then value of max-flow is $k$ .
> - **Max-flow Mini-cut Theorem** $\Rightarrow$ There exists a cut $(A, B)$ of capacity $k$ .
> - Let $F$ be the set of edges going from $A$ to $B$.
> - We can see that $|F| = k$ and it disconnects $t$ from $s$ .

通过证明 Menger's Theorem ，可以通过 Max-Flow 求解最大不相交路径的数量，该数量即为 Network Connectivity 问题的解。



## Disjoint Paths in Undirected Graphs

**Definition**

> - **Edge-disjoint paths problem in undirected graphs** - Given a graph $G = (V,E)$ and two nodes $s$ and $t$, find the max number of edge-disjoint $s \rightarrow t$ paths.
> - **Max-flow formulation** - Replace each edge with two antiparallel edges and assign unit capacity to every edge.
> - **Observation** - Two paths $P_1$ and $P_2$ may be edge-disjoint in the digraph but not edge-disjoint in the undirected graph (because of the anti-parallel edges) .

**Lemma** In any flow network, there exists a maximum flow $f$ in which for each pair of anti-parallel edge $e$ and $e'$: either $f(e) = 0$ or $f(e') = 0$ or both. Moreover, integrality theorem still holds.

> **Proof** [by induction on number of such pairs]
>
> - Suppose $f(e) > 0$ and $f(e') > 0$ for a pair of anti-parallel edges $e$ and $e'$ .
> - Set $f(e) = f(e) - \delta$ and $f(e') = f(e') - \delta$, where $\delta = \min \{f(e), f(e')\}$ .
> - $f$ is still a flow of the same value but has one fewer such pair.
>
> 这里没看明白，为什么流量减少之后，流函数 $f$ 的流量依旧不变？

**Theorem** Max number of edge-disjoint $s \rightarrow t$ paths = value of max-flow.

> **Proof** Similar to proof in digraphs, use the lemma above.





## Multiple Sources and Sinks

**Definition**

> Given a digraph $G = (V,E)$ with edge capacities $c(e) \ge 0$ and multiple source nodes and multiple sink nodes, find max flow that can be sent from the source nodes to the sink nodes.

**Max-flow formulation**

- Add a new source node $s$ and sink node $t$.
- For each original source node $s_i$ , add edge $s \rightarrow s_i$ with capacity $\infin$.
- For each original sink node $t_j$ , add edge $t_j \rightarrow t$ with capacity $\infin$ .



## Circulation with Supplies and Demands





## Survey Design





## Airline Scheduling





## Project Selection

