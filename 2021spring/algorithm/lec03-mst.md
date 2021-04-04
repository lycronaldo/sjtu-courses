## Lecture 03 - Minimum Spanning Tree

这节课逃课去聚餐了 #(逃 .

主要内容是最小生成树。

## Concepts

**Cut and cutset**

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210404203208.png" style="width:70%;" />

**Quiz** Let C be a cycle and let D be a cutset. How many edges do C and D have in common? Choose the best answer. (An even number)

**Tips**: 只需要考虑 Cut 中的点，在 Cycle 中是否相邻，即可证明。

从「图形」的直观上考虑，给定一个 Cut S，那么按照定义 S 中的边肯定属于 Cutset 。Cutset 中的边，一端在 S 内，另外一端在 S 外。考虑 S 中的某一个点，从 S 出发，在环上走一圈，每「出去」一次必然还是需要「回来」一次才能成环，因此交集总是偶数的。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210404204858.png" style="width:50%;" />

**Spanning Tree Definition**

Let $H = (V,T)$ be a subgraph of an undirected graph $G = (V,E)$. $H$ is a spanning tree of $G$ if $H$ is both acyclic and connected.

Properties:

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210404205501.png" style="width:67%;" />

**Cayley's Theorem**

完全图的生成树的数目为 $n^{n-2}$ .



**Fundamental Cycle**

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210404210653.png" style="width:67%;" />

**Fundamental Cutset**

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210404210722.png" style="width:67%;" />

Also have: If $c_e < c_f$, then $(V,T)$ is not a MST.



## Algorithm



### Prim



### Kruskal

