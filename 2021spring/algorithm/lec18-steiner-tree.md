## Metric Steiner Tree and Metric TSP

本节的主要内容是：介绍 Metric Steiner Tree 和 Metric TSP 问题的常数近似因子算法。

Metric 的含义包含 2 个层面的信息：

- 给定的图是完全图
- 对于任意三个不同的顶点，满足三角不等式：$cost(u, v) \le cost(u, w) + cost(w, v)$ 



## Metric Steiner Tree

Steiner Tree 即斯坦纳树：给定一个顶点集合 (Required Vertices) ，**允许增加额外的顶点**，求一棵树，使生成的最短网络开销最小（满足这样条件的树就叫斯坦纳树）。

对于「额外增加的顶点」，我们也可以指定一个集合，称为 Steiner Vertices.

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210602202609.png" style="width:80%;" />

那么可以这样描述斯坦纳树问题：给定图 $G = (V,E)$，要求将 $V$ 的一个子集 “Required Vertices” 连通（剩下的点就是 Steiner Vertices），求出最小权值。

Metric Steiner Tree 是一个 NPH 问题，最小生成树 MST 是斯坦纳树的一种特殊情况（可以在多项式时间内解决）。

如果加入一个限制条件：对于不同的三个顶点，$cost(u, v) \le cost(u, w) + cost(w, v)$ ，那么问题就称为 Metric Steiner Tree 。

---

Theorem - There is an approximation factor preserving reduction from the Steiner tree problem to the metric Steiner tree problem.

> **Proof**
>
> Firstly, we will transform in polynomial time, an instance $I$ of the Steiner Tree $G=(V,E)$, to an instance $I'$ of the metric Steiner Tree $G'$ .
>
> - Let $G'$ be the complete undirected graph on vertex set $V$. Define the cost of edge $(u, v)$ in $G'$ to be the cost of a shortest $u-v$ path in $G$. $G'$ is called the metric closure of $G$.
> - The partition of $V$ into **Required** Vertices and **Steiner** Vertices in $I'$ is same as $I$.
> - For any edge $(u, v) \in E$, its cost in $G'$ is no more than its cost in $G$. (This is obvious according to the above definition.)
> - Therefore, the cost of an optimal solution in $I'$ does not exceed the cost of an optimal solution in $I$.
>
> Next, given a Steiner tree $T'$ in $I'$, we will show how to obtain, in **polynomial** time a Steiner tree $T$ in $I$ of at most the same cost.
>
> - The cost of an edge $(u, v)$ in $G'$ corresponds to the cost of a path in $G$.
> - Replace each edge of $T'$ by the corresponding path to obtain a subgraph of $G$.
> - Clearly, in this subgraph, all the required vertices are connected. However, this subgraph may, in general, contain cycles.
> - If so, remove edges to obtain tree $T$ . This completes the approximation factor preserving reduction.
>
> As a consequence of this theorem, any approximation factor established for the metric Steiner tree problem carries over to the entire Steiner tree problem.

---



### MST-based algorithm

用 $R$ 表示 Required Vertices，一个很自然的想法：给定一个 Steiner Tree Problem 实例，我们在 $R$ 上做一遍 MST 算法 (Prim or Kruskal)，那么也能得到一个可行解，但这个树并不一定是最小的 Steiner Tree 。比如：

![](https://gitee.com/sinkinben/pic-go/raw/master/img/20210602212812.png)

黑色顶点表示集合 $R$ 。

但 MST-based Algorithm 是一个近似因子为 2 的近似算法（证明？不要问我怎么证明 😅 ）。

Theorem - The cost of an MST on $R$ is within $2 \cdot OPT$.

> **Proof**
>
> - Consider a Steiner tree of cost $OPT$. By doubling its edges we obtain an Eulerian graph connecting all vertices of $R$ and, possibly, some Steiner vertices.
> - Find an **Euler tour** of this graph, for example by traversing the edges in DFS (depth first search) order:
>
> ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20210602214554.png)
>
> - The cost of this Euler tour is $2 \cdot OPT$. Next obtain a Rudrata cycle on the vertices of $R$ by traversing the Euler tour and short-cutting Steiner vertices and previously visited vertices of $R$:
>
> ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20210602214732.png)
>
> - Because of triangle inequality, the shortcuts do not increase the cost of the tour.
> - This path is also a spanning tree on $R$. Hence, the MST on $R$ has cost at most $2 \cdot OPT$.



一个 Tight Example：

![](https://gitee.com/sinkinben/pic-go/raw/master/img/20210602215000.png)

其实就是上面给出的那个例子的一般情形。



## Metric TSP

