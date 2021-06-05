## Steiner Tree and TSP

本节的主要内容是：介绍 Metric Steiner Tree 和 Metric TSP 问题的常数近似因子算法。

Metric 的含义包含 2 个层面的信息：

- 给定的图是完全图
- 对于任意三个不同的顶点，满足三角不等式：$cost(u, v) \le cost(u, w) + cost(w, v)$ 



## Steiner Tree

Steiner Tree 即斯坦纳树：给定一个顶点集合 (Required Vertices) ，**允许增加额外的顶点**，求一棵树，使生成的最短网络开销最小（满足这样条件的树就叫斯坦纳树）。

对于「额外增加的顶点」，我们也可以指定一个集合，称为 Steiner Vertices.

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210602202609.png" style="width:80%;" />

那么可以这样描述斯坦纳树问题：给定图 $G = (V,E)$，要求将 $V$ 的一个子集 “Required Vertices” 连通（剩下的点就是 Steiner Vertices），求出最小权值。

Steiner Tree 是一个 NPH 问题，最小生成树 MST 是斯坦纳树的一种特殊情况（可以在多项式时间内解决）。

如果加入一个限制条件：对于不同的三个顶点，$cost(u, v) \le cost(u, w) + cost(w, v)$ ，那么问题就称为 **Metric Steiner Tree** 。

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

其中，黑色顶点表示集合 $R$ 。

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



## TSP

- Original TSP: 完全图，但边权值不满足三角不等式。
- Metric TSP: 完全图，边权值满足三角不等式。



### Original TSP

- TSP 问题：给定一个**完全图**，边权值均为非负数，找到最小权值的一个哈密顿环（遍历所有的点有且仅有一次）。
- 如果 $P \ne NP$ , 一般而言，TSP 问题是无法近似估计的。

Theorem - For any polynomial time computable function $\alpha(n)$, TSP can not be approximated within a factor of $\alpha(n)$, unless $P = NP$.

> **Proof**
>
> - Assume, for a contradiction, that there is a factor $\alpha(n)$ polynomial time approximation algorithm, $\mathcal{A}$, for the general TSP problem. 
> - We will show that $\mathcal{A}$ can be used for **deciding** the Hamiltonian cycle problem (which is NP-hard) in polynomial time, thus implying $P = NP$.
> - The central idea is a reduction from the **Hamiltonian cycle (HC, aka. Rudrata cycle, with a non-complete graph)** problem to TSP, that transforms a **graph** $G$ on $n$ vertices to an edge-weighted **complete graph** $G'$ on $n$ vertices such that:
>   - if $G$ has a Hamiltonian cycle, then the cost of an optimal TSP tour in $G'$ is $n$, and
>   - if $G$ does not have a Hamiltonian cycle, then an optimal TSP tour in $G'$ is of cost $> \alpha(n) \cdot n$ (and it's still polynomial time)
> - Since we reduce $\text{HC} \rightarrow \text{TSP}$, then we can say that TSP is "harder" than HC. Therefore, approximation algorithm $\mathcal{A}$ used for TSP can be also used for the HC problem.
> - At both of these two cases, algorithm $\mathcal{A}$ can be used for deciding whether $G$ contains a Hamiltonian cycle. Now polynomial time algorithm $\mathcal{A}$ can be used to solve HC. This is in contradiction with "HC is NPC".
>
> Now we show how to reduce HC to TSP.
>
> - $G'$ is a complete graph, we call $E_{G'} - E_G$ the "non-edges".
> - Assign a weight of 1 to edges of $G$ , and a weight of $\alpha(n)\cdot n$  to non-edges to obtain $G'$.
> - Now if $G$ has a HC, then the corresponding tour in $G'$ has cost $n$. 
> - Otherwise, $G$ does not have HC, any tour in $G'$ must use (at least) an edge cost $\alpha(n) \cdot n$, thus we have cost $> \alpha(n) \cdot n$.



### Metric TSP

虽然 TSP 是无法近似估计的，但满足三角不等式的 Metric TSP 是可以的。

下面给出一个近似因子为 2 的近似算法。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603115849.png" style="width:67%;" />

Notice that Step 4 is similar to the “short-cutting” step in MST-based algorithm.

Theorem - This is a factor 2 approximation algorithm.

> **Proof**
>
> - Obviously, we have $cost(T) \le OPT$ . (Since $T$ has $n-1$ edges and $OPT$ has $n$ edges.)
> - Since $\mathcal{T}$ contains each edge of $T$ twice, thus $cost(\mathcal{T}) = 2cost(T)$ .
> - Because of triangle inequality, after the "short-cutting" step, we have
>
> $$
> cost(\mathcal{C}) \le cost(\mathcal{T}) = 2cost(T) \le 2 \cdot OPT
> $$

一个 Tight Example 是：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603144859.png" style="width:80%;" />

其中，左边的图是给定的 $G$，右边的图是最优的 TSP Tour。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603145131.png" style="width:80%;" />

😅 虽然是差了一个常数，但依然认为这个例子里面的 $2n-2$ 是 2 倍的 OPT 。



### Improving the Factor to 3/2

上面提出并证明了一个近似因子为 2 的算法，这一小节把近似因子优化为 3/2. 

- Is there a **cheaper Euler tour** than that found by doubling an MST?
- Recall that a graph has an Euler tour **iff** all its vertices have **even degrees**.
- Thus, we only need to be concerned about the vertices of **odd degree in the MST**.

$V'$ 表示 MST 中的奇度顶点，显然 $|V'|$ 必然是个偶数，因为 $\sum_{e \in MST} deg(v)$ 等于 MST 边数的 2 倍。

现在，不需要 "double" MST 的所有边，只需要 "double" 与 $V'$ 「有关」的边（怎么才是「有关」见下面的算法）。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603150554.png" style="width:80%;" />

可以证明：这是一个近似因子为 3/2 的近似算法。

先证明 $cost(M) \le OPT/2$ .

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603151449.png" style="width:67%;" />

再证明 $cost(\mathcal{C}) \le cost(\mathcal{T}) \le \frac{3}{2} OPT$ .

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603151740.png" style="width:67%;" />

一个 Tight Example 如下：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603152142.png" style="width:67%;" />

这神奇的构造方法，特别是那条 $\lceil n/2 \rceil$ 的边。我看不懂，但我大受震撼。

终于又看完一个 slide 了，感天动地。

## References

- [1] Vaz - Approximation Algorithms (Chapter 3)

