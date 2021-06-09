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



## Maximum flow

给定源点 $s$ 和汇点 $t$，对于图 $G = (V, E)$ 的一个流函数 $f$，其流量值为从 $s$ 出去的量减去流入 $s$ 的量（大部分情况是 0 ），即： $ val(f) = \sum_{v \in V} f_{sv} - \sum_{v \in V} f_{vs}$ 。

那么其线性规划形式为：
$$
\begin{aligned}
& \text{ maximize }  &\sum_{v \in V} f_{sv} - \sum_{v \in V} f_{vs} \\
& \text{ subject to } & f_{uv} \le c(u, v)  & \text{ for each } u, v \in V \\
& & \sum_{v \in V} f_{vu} = \sum_{v \in V} f_{uv}  & \text { for each } u \in V - \{s,t\} \\
& & f_{uv} \ge 0 & \text{ for each } u, v \in V
\end{aligned}
$$
该 LP 问题：

- 最多具有 $V^2$ 个变量（意味着 $G$ 为完全图，如果不是，那么增添容量为 0 的边，转换为等价的完全图）；
- 最多具有 $2V^2+V-2$ 个约束条件。

一个例子：

|                           Example                            |                              LP                              |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210521125355.png" style="" /> | <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210521125427.png"  /> |



注意到上述 LP 的第二个约束条件，我们必须排除 $s,t$ 这 2 个特殊点。

如果我们添加一个虚拟边 $t \rightarrow s$ ，那么可以有：
$$
\begin{aligned}
& \text{ maximize } & f_{ts} \\
& \text{ subject to } & f_{ij} \le c_{ij}, &\text{ for } (i,j) \in E \\
& & \sum_{(w,i) \in E}f_{wi} - \sum_{(i,z) \in E} f_{iz} \le 0, &\text{ for } i \in E \\
& & f_{ij} \ge 0, & \text{ for } (i,j) \in E
\end{aligned}
$$


## Min-Max Relations and Duality

本节介绍 LP 的对偶问题，及其二者之间的关系。

- Vertex Cover



## References

- [1] CLRS - Introduction to Algorithm (Section 29.2)
- [2] WS11 - The Design of Algorithm (Section 7.3)