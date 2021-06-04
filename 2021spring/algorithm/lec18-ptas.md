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

个人对这个 FPTAS 的理解。一个算法的近似解的界，它依赖于 $\mathcal{E}$ ，同时这个算法的复杂度也跟 $1 / \mathcal{E}$ 有关，比如 $O(n/\mathcal{E})$ 。这意味着什么呢？以 $f(I,s) \le (1 + \mathcal{E}) \cdot OPT$ 为例子，如果我们对近似算法的要求放宽，允许它有更高的上界，即 $\mathcal{E}$ 增大，那么这个近似算法的复杂度会以 $1 / \mathcal{E}$ 的形式降低。

## Knapsack

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210603154949.png" style="width:67%;" />

- 贪心：把物品根据 `profit[i]/size[i]` 进行降序排列，然后根据排序后的序列进选取。
  - 显然是不行的，比如 `profit = [1, 99], size = [1, 100], B = 100` .
  - 或者：`profit = [1, B-1], size = [1, B] (B >= 2)` .
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

时间复杂度均为 $O(nB)$ ，$B$ 是背包容量。下面介绍一种 DP 解法，它是通过利润（而不是容量）来描述算法复杂度的。

- 令 $P$ 给定 profit 数值中的最大值，那么显然最优解的上界是 $nP$ .
- 对于 $i \in [1, n]$ 和 $p \in [1, nP]$，令 $S_{i,p}$ 表示物品 $[a_1, \dots, a_i]$ 的一个 **最小** 子集，它们的利润和 **恰好** 为 $p$ 。（即：我们的状态定义是放入尽可能少的物品，得到利润 $p$ ）
- $A(i, p)$ 表示 $S_{i,p}$ 这个子集的大小，显然边界条件 $A(1, p) = |S_{1, p}|, p \in [1, nP]$ 。
- 转移方程为：

$$
A(i+1, p) = \left\{ 
\begin{aligned}
& \min\{ A(i,p), size(a_{i+1}) + A(i, p - profit(a_{i+1})) \}, & \text{ if } p \le profit(a_{i+1}) \\
& A(i,p), & \text{ otherwise }
\end{aligned}
\right.
$$

- 最后所求的最大利润是 $\max\{p | A(n, p) \le B\}$ .
- 显然，这么做的算法复杂度为 $O(n^2P)$ .



### An FPTAS for Knapsack

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210604152930.png" style="" />

$P$ 是给定 profit 数组中的最大利润，显然，我们有 $OPT \ge P$ ，为什么呢？如果 $OPT < P$ ，那么我们只需要选取物品 $P$ 就能得到一个更大的利润，与 $OPT$ 是最优解矛盾。

- Lemma - Let $A$ denote the set output by the algorithm, then $profit(A) \ge (1 - \varepsilon) \cdot OPT$ .

> **Proof**
>
> That is to prove that the $profit(S') \ge (1 - \varepsilon) \cdot OPT$ where $S'$ is produced by the algorithm above.
>
> - Let $O$ denote the optimal set of the original knapsack problem. 
> - For any object $a$, because of rounding down, $K \cdot profit'(a_i)$ should be smaller than $profit(a_i)$, but by no more than $K$. Therefore, 
>
> $$
> profit(S') \ge K \cdot profit'(S') \\
> profit(O) - K \cdot profit'(O) \le nK
> $$
>
> - The dynamic programming algorithm **must return the set $S'$ (at least) as good as $O$** under the new profits (since DP is proved that it can find the optimal solution of Knapsack Problem) , i.e. $profit'(S') = profit'(O)$ .Therefore,
>
> $$
> \begin{aligned}
> profit(S') & \ge K \cdot profit'(S') = K \cdot profit'(O) \\
> & \ge profit(O) - nK = OPT - \varepsilon P \\
> & \ge OPT - \varepsilon \cdot OPT \\
> & = (1-\varepsilon) \cdot OPT
> \end{aligned}
> $$

- Theorem - This algorithm is FPTAS for knapsack.

> **Proof**
>
> - By lemma described above, the solution found is with $(1-\varepsilon) \cdot OPT$ .
> - Since the running time of this algorithm is $O(n^2 \lfloor \frac{P}{K} \rfloor) = O(n^2 \lfloor \frac{n}{\varepsilon} \rfloor)$ , which is polynomial in $n$ and $1/\varepsilon$ .
> - Thus the algorithm is FPTAS.



## Bin Packing

给定 $n$ 个物品 $a_1, \dots, a_n$ ，每件物品的体积均在 $(0, 1]$ 范围内，所有箱子的容量均为 1 ，要求使用最少的箱子装入所有的物品。

这是一个 NPC 问题，下面介绍一个近似因子为 2 的近似算法：

- 任意选取一个 $a_i$ ，假设当前已经使用的箱子为 $B_1, \dots, B_k$ 。
- 依次扫描 $B_1$ 到 $B_k$ ，找到一个能放入 $a_i$ 的箱子，否则打开一个新箱子 $B_{k+1}$  。
- 重复上述过程，知道所有物品都被装入。

下面证明其近似因子为 2 。

- 假设最优解为 $OPT = B^*$ ，上述算法需要 $B$ 个箱子。
- 在上述算法中，至少会有 $B-1$ 个箱子是半满的，即已装入的容量严格大于 $0.5$ 。这是因为：如果存在 2 个箱子，它们的装入量均 $\le 0.5$ ，那么根据算法，后一个箱子的物品应该放入前一个箱子当中。那么有：

$$
B^* \ge \sum_{i=1}^{n}a_i > \frac{B-1}{2}
$$

- 因此有：$B < 2B^*+1$ .

---

![](https://gitee.com/sinkinben/pic-go/raw/master/img/20210604162725.png)





## APTAS

TBD：我看不懂，但我大受震撼。



## References

- [1] Vaz04 - Approximation Algorithms (Chapter 8 and Chapter 9)
- [2] WS11 - The Design of Approximation Algorithms (Section 3.3)

