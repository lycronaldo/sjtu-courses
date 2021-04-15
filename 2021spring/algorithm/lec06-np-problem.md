## Lec06 - NP Problem

考试透露：规约 (Reduction) 是必考的。



- P：多项式时间求解
- NP：多项式时间验证

P 是 NP 的子集。



## Some Problems

**Set Cover**

即集合覆盖问题。

> - Input: A set of elements $B$, sets $S_1, \dots, S_m \sube B$ .
> - Output: A selection of $S_i$ whose union is $B$ .
> - Cost: Number of sets picked.

与之类似的问题还有 Subset Sum: 

> Find a subset of a given set of integers that adds up to exactly $W$.



**Graph Isomorphism**

图同构问题。

寻找图 $G$ 和图 $H$ 两个图的点集之间的双射 $f: V(G) \rightarrow V(H)$ ，且满足：

> $\forall{x,y \in V(G)}$ 在 $G$ 中是相邻的 iff $\forall {f(u), f(v) \in V(H)}$ 在 $H$ 中是相邻的

用大白话来说就是：把 $G$ 中的点重新「标号」，得到 $H$，那么 $G$ 和 $H$ 就是同构的。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210415140634.png" style="width:60%;" />

Set Cover 和 Graph Isomorphism 都是 NP-Complete 问题。



## P and NP

参考资料：

- [1] [Complexity Class](https://brilliant.org/wiki/complexity-classes/)
- [2] [NP complexity](https://en.wikipedia.org/wiki/NP_(complexity))



P 是指 Polynomial 这个单词，NP 当然**不是** Non-polynomial（NP 其实是 [Non-deterministic Polynomial time](https://en.wikipedia.org/wiki/NP_(complexity)) ）。

这一节主要涉及 4 类问题：P, NP, NPC, NPH. 这 4 个名词实际上叫复杂类 (Complexity Class) ，属于理论计算机科学 (Theoretical Computer Science, TCS) 的研究范畴。那什么叫 Complexity Class 呢？

> A complexity class contains a set of problems that take a similar range of space and time to solve.

回顾上一节 (lec05) ，提到判定问题 (Decision Problem) ：可以在**多项式**时间内回到 YES 或者 NO 的问题。

那么可以给出上述 4 类问题的自然语言描述：

- **P** is a complexity class that represents the set of all decision problems that can be **solved in polynomial time**.
- **NP** is a complexity class that represents the set of all decision problems for which the instances where the answer is **"yes"** have proofs that can be **verified in polynomial time**.
  - 虽然能够在多项式时间内验证，但目前无法在多项式时间内解决，例如 TSP 和 SAT 问题。
- **NP-Complete(NPC)** is a complexity class which represents the set of any problem in the NP class can be transformed or reduce into NP-Complete problems in polynomial time. (or, a NP problem is NPC if all other NP problems reduce to it.)
  - NPC 是 NP 的一个子集，它包含了这样的元素：NP 中的任何问题都可以多项式时间内规约 (Reduce) 到该子集的某一个元素（也就是说，NPC 是把 NP 中的所有问题经过规约、合并后得到的集合）。
  - 这意味着：如果能解决一个 NPC 问题，那么所有的 NP 问题都能解决；如果能在多项式时间一个 NPC 问题，那么所有的 NP 都能在多项式时间内解决，那么就能证明 $P=NP$ 。
  - SAT 问题是一个 NPC 问题 (aka, Cook-Levin Theorem) 。
- **NPH (NP-Hard)**: these are the problems that are at least as hard as the NP-Complete problems.
  - NPH 是与 NPC 同等困难的问题（即「可多项式验证但无法多项式解决」）
  - NPH 问题不一定是判定问题（因此也不一定是 NP 问题）



根据定义，可以得到一些常见的结论：

- $P \sub NP$ （但是否 $P = NP$ 尚未解决）
- NPC 是 NPH 和 NP 的交集 



来自 Refs [1] 的一张图：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210415152832.png"  style="width:70%;" />

如果 $P = NP$ ，显然 $NPC$ 也能在多项式内解决，因此 $P=NP=NPC$ .



## coNP

首先给出 Complementation 的定义：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210415154917.png" style="width:60%;" />

coNP 的自然描述：

> If a problem X is in NP, then its complement, $\overline{X}$ is in coNP. coNP contains problems that have **a polynomial time verification for “no”** answers - if given a solution that does not solve the problem, it is easy to verify if that solution does not work.

与 NP 的区别：NP 能在多项式时间内验证 YES，coNP 能在多项式时间内验证 NO 。

coNP = NP ？ 目前尚未解决。

与之对应的，可以定义 coNPC 问题：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210415161114.png" style="width:67%;" />

## Reduction

$h$ 可以视作是 $f$ 的逆映射，即 $h = f^{-1}$ .

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210415160304.png"  style="width:67%;" />

关于 Reduction 的性质：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210415160530.png"  style="width:67%;" />

