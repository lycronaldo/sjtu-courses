## Linear Programming: Application

- Shortest Path
- Max-flow Problem





## Shortest Path

最短路径算法最著名的是 Dijkstra 算法和 Bellman-Ford 算法。

**Formalization - 1**

来源于 Refs [1] ，基于 Bellman-Ford 算法的思想，将最短路径问题规约到 LP 问题。

给定 $G=(V,E)$ ，起点 $s$ 和终点 $t$ ，定义 $d_v$ 为 $s$ 到 $v$ 的最短距离，那么我们有：
$$
\begin{aligned}
& \max d_t \\
& d_v \le d_u + w(u,v), \text{ for each edge } (u, v) \in E \\
& d_s = 0 \\
& d_i \ge 0, \text{ for any } i \in V
\end{aligned}
$$


至于为什么目标函数是 $max$ ，我是真的看不懂了。



**Formalization - 2**

来源于 Refs [2] ，基于 Dijkstra 算法的规约。

> Let $\Gamma = \{S \sube V: s \in S, t \notin S\}$ , that is $\Gamma$ is the set of all $s-t$ cuts in the graph. Then we can model the shortest $s-t$ path problem with the following integer problem:
> $$
> \begin{aligned}
> & \text{minimize }     \sum_{e \in E} w_ex_e \\
> & \sum_{e \in \delta(S)} x_e \ge 1, & \forall{S \in \Gamma} \\
> & x_e \in \{0, 1\}, & \forall{e \in E}
> \end{aligned}
> $$
> where $\delta(S)$ is the set of all edges that have one endpoint in $S$ and the other endpoint not in $S$.

其对偶形式（其中这个花体 $\textbf{S}$ 就是上述的 $\Gamma$）：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210514155118.png" style="width:67%;" />

😅 看不懂，看不懂。



## Max-flow





## References

- [1] CLRS - Introduction to Algorithm (Section 29.2)
- [2] WS11 - The Design of Algorithm (Section 7.3)