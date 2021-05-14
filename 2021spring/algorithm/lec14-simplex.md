## Simplex Algorithm

本节的主要内容是 Simplex Algorithm ，一个解决 Linear Programming 问题的算法。



## Introdcution

Simplex 算法的主要思路为：
$$
\begin{aligned}
& \text{let } v \text{ be any vertex of the feasible region} \\
& \text{while there is a neighbor } v′ \text{ of } v \text{ with better objective value:} \\
& \quad \quad \text{set } v = v'
\end{aligned}
$$
考虑一个具有 $n$ 个变量 $\bar{x} = (x_1, \dots, x_n)$  的**标准型**的线性规划问题，一个关键的问题是：我们如何定义 Neighbor Vertex ？

当 $n = 2$ 时，可行域是一个多边形，$n=3$ 时可行域是一个多面体。那么如果 $n \ge 4$ 时，在一个 $n$ 维的空间中，我们如何定义 Neighbor  Vertex ？





## Simplex







## References

- [1] DPV - Algorithm (Section 7.6)
- [2] CLRS - Introduction to Algorithm (Section 29.3)