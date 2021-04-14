## Lec05 - Various Problem

这一节的内容太顶了。



## Efficient/Diffcult Problem

Efficient algorithms:
- Finding shortest paths in graphs
- Minimum spanning trees in graphs
- Matchings in bipartite graphs
- Maximum increasing subsequences
- Maximum flows in networks

All these algorithms are efficient, since their time requirement grows as a **polynomial function** of the size of the input.

Difficult Problem: seeking a solution among exponential space, and the fastest algorithm is **exponential function** time complexity.



## SAT

SAT (Satisfiability) 问题是一个经典的判定问题，且是第一个被证明为 NP 完全的问题。

在 SAT 问题中：

- 变量 (Variable): 给定 $n$ 个变量 $V = \{x_1, x_2, \dots, x_n\}$ ，其中每个变量都是 boolean 类型的，只能为 1 或者 0 .
- 文字 (Literal): 由 $x_i$ 加上逻辑非 $\lnot$ 运算符组成的符号 $x_i$ 和 $\lnot x_i$，文字集合 $L = \{x_1, \lnot{x_1}, \dots, x_n, \lnot {x_n}\}$ .
  - $x_i$ 称为 Positive Literal，$\lnot{x_i}$ 称为 Negative Literal 。
- 子句 (Clause): 若干个 Literal 经过析取操作 (Disjunction) 构成，例如 $c = x_1 \lor x_2 \lor (\lnot{x_5} )$ ，子句的长度就是文字的个数。析取实际上就是 or 运算。
- 合取范式 (Conjunctive Normal Form, CNF): 若干个子句经过合取构成，例如 $c_1 \land c_2 \land c_3$ 。合取实际上是 and 运算。

一个 CNF 可以看作是一个变量集合 $V$ 和一个子句集合 $C$ 组成的二元组。显然，每一个子句都只能为 true 或者 false。

SAT 问题：是否存在一个对 $V$ 的赋值方案，使得 CNF 表达式为真。显然，SAT 问题的状态空间是 $O(2^n)$ 的。

如果一个 CNF 的每一个 Clause 最多只有一个 Positive Literal ，那么就称为 **Horn Formula** (aka, [Horn Clause](https://simple.wikipedia.org/wiki/Horn_clause)) .

Horn Formula 可以通过贪心算法求解。



**k-SAT**

CNF 表达式中的每一个子句的长度都是 $k$ ，但变量集合的大小可以是 $n(n>k)$ 。



**2-SAT**

当 $k=2$ 时，2-SAT 问题是可以在 $O(n)$ 时间内解决。

给定一个变量个数为 $n$ 的 2-SAT 表达式 $CNF = c_0 \lor c_1 \lor \dots \lor c_{m-1}$，其变量集合为 $\{x_0, \dots, x_{n-1}\}$，构造一个图 $G=(V,E)$:

- $|V| = 2n$ ，把每个 $x_i$ 拆分为 2 个节点，标号为 $2i$ 和 $2i+1$ 。
- $|E|=2m$ ，对于一个子句 $c_i = \alpha \lor \beta$，构造 2 条边 $\lnot{\alpha} \rightarrow \beta$ 和 $\lnot{\beta} \rightarrow \alpha$ .
  - 边 $i \rightarrow j$ 表示如果选择 $i$ ，那么不能选择 $j$ 。

可以证明：

- 如果 $G$ 的某个强连通分量，同时包含 $x_i$ 和 $\lnot{x_i}$，那么 $G$ 对应的 2-SAT 问题无解。
- 如果 $G$ 的所有强连通分量，都不同时包含 $x_i$ 和 $\lnot{x_i}$，那么 $G$ 对应的 2-SAT 问题有解。

强连通分量问题，可以通过 Tarjan 算法求解。



## Search/Decision Problem

Decision Problem (aka, Search Problem)，亦即所谓的「决定性问题」或「判定问题」，只需回答 True 或者 False 的一类问题。

课件中把 Search Problem 和 Decision Problem 放在一起讲，似乎把二者当作同义词，虽然我没找到相关的文献去阐述这一点。但从名词的语义上理解，它们都是在做一件事情：是否存在 $x$ 满足 $f(x)$，存在则输出这个 $x$ ，否则输出 false 。

Search/Decision Problem 的定义：

>- A search problem must have the property that any proposed solution *S* to an instance *I* can be quickly checked for correctness.
>- *S* must be concise, with length **polynomially** bounded by that of *I*.

上述的 SAT 问题就是一个典型的 Search Problem ，因为：如果给定一组变量赋值 $S$，可在 $O(N)$ 时间内完成检验，检验 $S$ 是否为 SAT 问题的解。



## TSP

给定一个完全图 $G$，有 $n$ 个点和 $n(n-1)/2$ 条带权边，以及一个预算 $b$ 。求：是否存在哈密顿回路（经过每个点一次并回到起点的路径），并且这个哈密顿回路的权值小于等于 $b$ ，若不存在，输出 False 。

对于这一问题，暴力枚举的状态空间是 $O(n!)$ ，可以通过状压 DP 优化到指数级，参考：https://www.cnblogs.com/sinkinben/p/14350513.html 。

>**Aside**
>如果在 MST 问题中，限制树结构不允许有分支（即只能有一个子节点），那么这样的 MST 就是一个哈密顿路径。

TSP 是一个典型的 Search/Decision Problem ，因为给定一个环路，可以在线性时间内完成检验。

TSP 同时也是一个优化问题 (Optimization Problem) ，即：在 $G$ 的所有哈密顿环路中，求出权值最小的哈密顿环路。



## Optimization Problem

Optimization Problem 即中文里面的「优化问题」或者「最优化问题」。

参考 Wikipedia 的 [Optimization Problem](https://en.wikipedia.org/wiki/Optimization_problem) 词条。

优化问题的定义：

> An optimization problem is the problem of finding the best solution from all feasible solutions.

以变量是离散变量还是连续变量为依据，分为离散优化 [Discrete Optimization](https://en.wikipedia.org/wiki/Discrete_optimization) 和连续优化 [Continuous Optimization](https://en.wikipedia.org/wiki/Continuous_optimization) 。

> - An optimization problem with discrete variables is known as a **discrete optimization**, in which an object such as an [integer](https://en.wikipedia.org/wiki/Integer), [permutation](https://en.wikipedia.org/wiki/Permutation) or [graph](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)) must be found from a [countable set](https://en.wikipedia.org/wiki/Countable_set).
> - A problem with continuous variables is known as a **continuous optimization**, in which an optimal value from a [continuous function](https://en.wikipedia.org/wiki/Continuous_function) must be found. They can include [constrained problems](https://en.wikipedia.org/wiki/Constrained_optimization) and multimodal problems.

判定问题与优化问题是可以相互转换的，解决一方即可解决另外一方，以上述 TSP 为例子：

- 如果解决了 TSP 的优化问题，即找出了权值最小的哈密顿环路。那么：只需要判断该哈密顿环路的权值是否小于等于预算 $b$ ，即可解决 TSP 的判定问题。
- 如果解决了 TSP 的判定问题，即找到一个算法 $F(x)$ 可以判定 $G$ 中是否存在权值小于等于 $x$ 的哈密顿环路。
  - 显然，哈密顿环路的权值的范围是 `[0, sum_weight]` ，其中 `sum_weight` 是所有边的权值之和。
  - 那么：可以对区间 `[0, sum_weight]` 从小到大枚举（或者二分查找），可以找到满足 $F(x)$ 的最小 $x$ ，即解决了 TSP 的优化问题。

对于任意的判定问题、优化问题，都存在上述形式的转换。



## Euler Path

Euler Path，即欧拉路径。

> Given a graph, find a path that contains each edge exactly once.

同样地，如果欧拉路径能回到起点，那么就叫欧拉回路 (Euler Cycle)。哈密顿路径/回路是「点遍历」，而欧拉路径/回路是「边遍历」。

无向连通图 $G$ 的欧拉定理：

- 无向连通图 $G$ 存在欧拉回路的充要条件是： $G$ 的每个顶点的度数都是偶数。
- 无向连通图 $G$ 存在欧拉路径的充要条件是： $G$ 中度数为奇数的顶点数目是 0 或 2 。

有向连通图 $G$ 的欧拉定理：

- 存在欧拉回路：所有顶点的出度与入度相等。
- 存在欧拉路径：一个顶点出度比入度大 1（起点），一个顶点入度比出度大 1（终点），其他点度数均为偶数。

欧拉回路/路径问题可以在多项式时间内解决：DFS 或者 Fleury 算法。

Fleury 算法的要点简记：走过的边从 $G$ 中删除；**优先选择**与当前节点相邻的非桥边。



## Rudrata Cycle

在国际象棋中，骑士 Knight 每一步的走法是：先直走一格，再斜走一格（类似于中国象棋的「马走日」）。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210414170918.png" style="" />

Rudrata Cycle 问题： 棋盘是否存在一个环路，骑士遍历棋盘的所有格子，然后回到起点。

Rudrata Cycle 其实是 TSP 环路问题的一半形式，将棋盘上的每个格子看作一个点，定义 `getid(i, j) = i*cols+j` ，那么点 `(i, j)` 与下面的 8 个点相连（如果它们没有超出棋盘）：

```
up    : (i-2, j-1), (i-2, j+1)
down  : (i+2, j-1), (i+2, j+1)
left  : (i-1, j-2), (i+1, j-2)
right : (i-1, j+2), (i+1, j+2)
```

那么 Rudrata Cycle 问题是在上述的图中寻找一个哈密顿回路。与 TSP 环路问题不同的是：TSP 的图是完全图，这里的图不是完全图。



## Minimum Cut

Minimum Cut，即最小割问题，其判定问题即：在图中是否存在一个边割集 $S$，$S$ 的权重不超过预算 $b$ 。

最小割问题可以转换为**最大流问题**解决，可以在多项式时间内完成（参考 Assignment-1 的第 6 题）。



**点割集**

设 $V' \subset V$ 使得无向图去掉 $V’$ 的所有点后，图的连通分支增加，但去除 $V'$ 的一个真子集，连通分支不增加，这样的 $V$ 称为「点割集」。如果 $V’$ 只有一个点，那么该点称为「割点」。

例子：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210414173511.png" style="width:80%;" />

**边割集**

与「点割集」的定义类似。边割集只有一个边时，该边称为「桥」或者「割边」。





## Balanced Cut

Balanced Cut，即平衡割问题。

> Given a graph with $n$ vertices and a budget $b$, partition the vertices into two sets $S$ and $T$ such that $|S|,|T| \ge n/3$ and such that there are at most $b$ edges between $S$ and $T$ .



## Integer Linear Programming

[Integer Linear Programming](https://en.wikipedia.org/wiki/Integer_programming) (ILP) 问题，即整数线性规划问题，是离散优化问题（上面有提到的）的一个分支（其实就是高中数学说的线性规划）。

ILP 问题：给定 $n$ 个变量和与之相关 $m$ 个不等式，求出目标函数 $F(x_1, \dots, x_n)$ 的最大值（最小值）。

如果从线性代数的角度来看：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210414175714.png"  style="width:65%;" />

其中：

- $\bf{x}$ 是 $n$ 个变量组成的 $n \times 1$ 的矩阵，也叫一个 vector 。
- $c \cdot \bf{x}$ 是目标函数
- $g$ 是所求的最值



## 3D Matching

三维匹配问题：给定三个不相交的集合 $X,Y,Z$ , 集合大小均为 $n$ 。给定三元组集合 $T \sube X \times Y \times Z$ ，集合 $T$ 的大小为 $m$ 。求：$T$ 中是否存在一个大小为 $n$ 的子集 $T'$ ，$T‘$ 恰好包含 $X,Y,Z$ 的每个元素一次。

与二维匹配类似，可能在具体的问题场景更好理解：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210414190849.png" style="width:70%;" />



## Independent Set, Vertex Cover and Clique

此处保留一个问题：Independent Set, Vertex Cover 和 Clique，三者的相同点/不同点在哪？



**独立集**

Independent Set，即独立集（也称为「稳定集」），一个图中一些两两不相邻的顶点所形成的集合，如果两个点没有公共边，那么这两个点可以被放到一个独立集中。等价于：

- 图中的每条边，至多有一个端点在独立集 $S$ 中。
- 独立集 $S$ 的任意 2 个点都不相连。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210414191614.png" style="width:60%;" />



**最大独立集**

极大独立集：设 $G$ 的一个独立集为 $S$，如果向 $S$ 中再添加一个点 $v$，独立性会丧失，即 $S \cup \{v\}$ 不是独立集，那么 $S$ 称为「极大独立集」。

最大独立集：如果 $S$ 是所有独立集中基数最大的，那么称 $S$ 是最大独立集，并把 $S$ 的基数记为 $G$ 的独立数，用 $\alpha{(G)}$ 表示。

寻找最大独立集问题是 NP 困难的优化问题。

 

**点覆盖**

Vertex Cover，即点覆盖问题。在图 $G$ 中寻找一个点集 $S$，$S$ 能覆盖所有的边。最小点覆盖问题则是要求 $S$ 中的点数最少。



**团**

Clique，即团问题。

给定一个有 $n$ 顶点的图 $G=(V,E)$，以及一个非负整数 $k$ 。问：是否存在 $V' \sube V$ ，且 $|V'| \ge k$ ，使得 $\forall{x,y \in V'}$ ，都有 $(x,y) \in E$ 。即：在 $G$ 中寻找一个点数大于等于 $k$ 的完全图（当然也是 $G$ 的子图）。



## Knapsack

背包问题：给定 $n$ 个物品，其重量分别为 $\{w_1, \dots, w_n\}$ ，其价值分别为 $\{v_1, \dots, v_n\}$ ，问：在限制容量 $C$ 的条件下，能跟装入背包的物品都最大价值。

- **0-1 背包问题**：要求重量 $w_i$ 均为整数，并且每个物品只有一个（即只能放入背包一次）。
- **完全背包问题**：要求重量 $w_i$ 均为整数，每个物品有无限多个（可以多次放入背包）。

0-1 背包和完全背包是 DP 的经典题目了，可以在 $O(nC)$ 时间内解决（但目前还没有多项式时间的算法）。参考：https://www.cnblogs.com/sinkinben/p/11673555.html

如果对于每个物品，均有 $w_i = v_i$ ，那么问题就转换为：在集合 $V$ 中，找到一个子集，该子集之和为 $C$ 。这也是所谓的 Subset Sum 问题。



## Summary

几类问题的学术名词：

- 困难问题 (Difficult Problem): 目前只有指数级时间复杂度的算法，无法在多项式时间内解决的问题。
- 判定问题 (Search/Decision Problem): 又称搜索问题、决定性问题，**判定**是否存在 $x$ 满足 $F(x)$ ，若存在输出 $x$，否则输出 False 。
- 优化问题 (Optimization Problem): 又称最优化问题，从可行的解空间中找到满足约束条件的最优解。
  - 离散优化问题（例如 ILP）
  - 连续优化问题



一些常见的问题：

- SAT 问题
  - 2-SAT 可以在线性时间解决
- TSP 问题（哈密顿路径/回路）
  - TSP 判定问题
  - TSP 优化问题
- 欧拉路径 / 回路：可以在多项式内解决，DFS 或者 Fleury 算法。
- Rudrata Cycle ：TSP 环路的一般形式，给定的图是非完全图。
- 最小割 Minimum Cut：转换为最大流问题解决。
- 平衡割问题
- 3D Matching
- 背包问题
  - 0-1 背包和完全背包
  - 当 $w_i = v_i$ 时，背包问题转换为 Subset Sum 问题



一些结论：

- 判定问题与优化问题可以相互转换。

