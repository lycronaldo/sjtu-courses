## PTAS

- PTAS: Polynomial Time Approximation Scheme
- FPTAS: Fully PTAS
- APTAS: Asymptotic polynomial-time approximation scheme (渐进多项式逼近)

这些名词真牛，我看不懂，但我大受震撼。



## Introduction

**Approximation Scheme**

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603154527.png" style="width:80%;" />

**PTAS and FPTAS**

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603154841.png" style="zoom:80%;" />



## Knapsack

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603154949.png" style="width:67%;" />

- 贪心：把物品根据 `profit[i]/size[i]` 进行降序排列，然后根据排序后的序列进选取。
  - 显然是不行的，比如 `profit = [1, 99], size = [1, 100], B = 100` .
- 动规：众所周知，背包问题是可以通过动态规划在 $O(nB)$ 时间内解决的，但这是一个伪多项式时间的算法。

下面介绍动规解法（我都写麻了）。

定义 $K(w,j)$ 为前 $j$ 个物品，放入容量 $w$ 的背包的最大价值。

如果物品不可重复，那么称为「01背包」：
$$
K(w, j) = \max\{ K(w, j-1), K(w-\text{size}(a_j), j-1) + \text{profit}(a_j) \}
$$



如果物品可重复，那么称为「完全背包」：
$$
K(w, j) = \max\{ K(w, j-1), K(w-\text{size}(a_j), j) + \text{profit}(a_j) \}
$$





## Bin Packing

给定 $n$ 个物品 $a_1, \dots, a_n$ ，每件物品的体积均在 $(0, 1]$ 范围内，所有箱子的容量均为 1 ，要求使用最少的箱子装入所有的物品。

这是一个 NPC 问题，下面介绍一个近似因子为 2 的近似算法：

- 任意选取一个 $a_i$ ，假设当前已经使用的箱子为 $B_1, \dots, B_k$ 。
- 依次扫描 $B_1$ 到 $B_k$ ，找到一个能放入 $a_i$ 的箱子，否则打开一个新箱子 $B_{k+1}$  。
- 重复上述过程，知道所有物品都被装入。

下面证明其近似因子为 2 。

- 假设最优解为 $B^*$ ，上述算法需要 $B$ 个箱子。
- 在上述算法中，至少会有 $B-1$ 个箱子是半满的，即已装入的容量严格大于 $0.5$ 。这是因为：如果存在 2 个箱子，它们的装入量均 $\le 0.5$ ，那么根据算法，后一个箱子的物品应该放入前一个箱子当中。那么有：

$$
B^* \ge \sum_{i=1}^{n}a_i > \frac{B-1}{2}
$$

- 因此有：$B < 2B^*+1$ .



## References

- [1] Vaz04 - Approximation Algorithms (Chapter 8 and Chapter 9)
- [2] WS11 - The Design of Approximation Algorithms (Section 3.3)

