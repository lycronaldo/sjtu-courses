## Linear Programming: Introduction

- 考试：对偶 Duality 是必考的，给定一个原线性规划，求出其对偶线性规划。
- 主要内容：线性规划问题，单纯形法 (Simplex Method)。



## Introduction

线性规划 (Linear Programming) 就是高中数学讲的那个线性规划，不过现在是从计算机的角度来谈这个问题的。

给定一个目标函数 (Objective function) ，和若干个不等式约束 (Constraints) ：
$$
\max{x_1 + 6x_2} \\
x_1 \le 200 \\
x_2 \le 300 \\
x_1 + x_2 \le 400 \\
x_1, x_2 \ge 0
$$


求出目标函数的最大值。

常规的做法是令 $c = x_1 + 6x_2$，然后可得 $x_2 = \frac{1}{6}(-x_1+c)$ ，通过作图法，找出截距 $c$ 的最大值。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210513150351.png" style="width:40%;" />

这里的可行域是一个封闭的区域，因此最优解必然存在，最优解不存在的 2 种情况是：

- 可行域是空的，比如 $x \le 1, x \ge 2$ .
- 可行域不是封闭的，比如目标函数为 $\max{x_1+x_2}$ ，约束条件为 $ x_1, x_2 \ge 0$ 。

但应当注意的是，即使可行域不是封闭的，最优解可能依然存在，比如目标函数为 $\max{x_1}$，而约束条件为 $x_1 \le 1, x_2 \ge 0$ .



## Simplex

LP 问题是 NP-Hard 的，单纯形法 (Simplex) 是一个解决 LP 问题的算法，将在下一篇文章详细介绍，在这里先给出一个直观的介绍。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210513151036.png" style="width:30%;" />

从可行域的一个有效点出发，若果存在一个相邻顶点使得目标函数具有更优值，那么令当前顶点为这个邻居顶点，重复这个过程直到不存在这样的更优邻居顶点。

为什么这样的局部测试 (Local Test) 会找到全局最优解呢？

> In this way it does **hill-climbing** on the vertices of the polygon, walking from neighbor to neighbor so as to steadily increase profit along the way. 
>
> By simple geometry - think of the profit line passing through this vertex. Since all the vertex’s neighbors lie below the line, the rest of the feasible polygon must also lie below this line.

如果存在 3 个变量，即：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210513152333.png" style="" />

那么 Simplex 的过程实际上是在一个凸多面体的边缘上进行 hill-climbing:

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210513152449.png" style="width:40%;" />



## Standard and slack forms

对于一个 LP 问题的实例，我们都能把它转换为一个标准型 (Standard Form) ：

- 目标函数是 $max$ 类型的。
- 所有变量都具有非负约束 (Nonnegativity constraints) 。
- 所有的约束条件都是 $\le$ 类型的不等式

然后对于任意的标准型 LP 问题，我们都能转换为 Slack Form 来解决，没找到中文翻译，暂且称之为「松弛型」。



### Standard form

标准型具有下列形式：
$$
\begin{aligned}
& \text{ maximize } & \sum_{j=1}^{n} c_jx_j \\
& \text{ subject to } &  \\
& & \sum_{j=1}^{n}a_{ij}x_j \le b_i \text{,  for } i=1,2, \dots, m \\
& & x_j \ge 0 \text{,  for } i=1,2, \dots, m
\end{aligned}
$$
如果写成矩阵形式：
$$
\begin{aligned}
& \text{ maximize } & \textbf{c}^T \textbf{x} \\
& \text{ subject to } & \\
& & \textbf{Ax} \le \textbf{b} \\
& & \textbf{x} \ge 0
\end{aligned}
$$
其中，$\textbf{c}^T$ 是一个行向量（即 $\textbf{c}$ 是一个列向量），$\textbf{b}$ 和 $\textbf{x}$ 均为列向量，$\textbf{A}$ 是一个矩阵，并且有 $|\textbf{A}| = m \times n$。

下面看如何任意一个 LP 问题转换为标准型。非标准型可能存在下列情况：

- 目标函数 $z$ 可能为 $\text{minimize } z$ 类型。
- 可能存在 $=, \ge$ 这样的约束条件。
- 可能存在某些变量 $x_i$ ，缺少了 $x_i \ge 0$ 这样的非负约束条件。

对于前面 2 种情况，处理方法是显而易见的：

- 把目标函数转换为 $\text{maximize } -z$ .
- $f(x) = k$ 转换为 $k \le f(x) \le k$ 的形式 。
- $f(x) \ge k$ 转换为 $-f(x) \le -k$ 的形式。

对于最后一种情况，对于缺少非负约束的 $x_i$ ：

- 令 $x_i = x_{i_{1}} - x_{i_2}$ ，并替换所有的 $x_i$ .
- 添加 2 个额外的约束：$x_{i_1}, x_{i_2} \ge 0$ .

其实这相当于坐标系的变换，以二维空间为例，令 $x = x - k$ 相当于把可行域往右移动了 $k(k\ge0)$ 个单位，对 $y$ 坐标做同样的操作，相当于把可行域向上移动，最终可使得可行域都位于第一象限，即所有变量都具有非负约束。

一个简单的例子：

|                            Primal                            |                           Standard                           |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20210515151325.png) | ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20210515151403.png) |



### Slack form

对于一个标准型的 LP ：
$$
\begin{aligned}
& \text{ maximize } & \sum_{j=1}^{n} c_jx_j \\
& \text{ subject to } &  \\
& & \sum_{j=1}^{n}a_{ij}x_j \le b_i \text{,  for } i=1,2, \dots, m \\
& & x_j \ge 0 \text{,  for } i=1,2, \dots, m
\end{aligned}
$$
令：
$$
\begin{aligned}
& s = b_i - \sum_{j=1}^{n} a_{ij}x_j \\
& s \ge 0 
\end{aligned}
$$


即转换为：
$$
\begin{aligned}
& \text{ maximize } & z = v + \sum_{j=1}^{n} c_jx_j \\
& \text{ subject to } &  \\
& & x_i = b_i - \sum_{j=1}^{n}a_{ij}x_j, \text{ for } i=n+1,\dots, n+m \\
& & x_i, x_j \ge 0, \text{  for any } i \text{ or } j
\end{aligned}
$$
一个简单的例子：

|                           Standard                           |                            Slack                             |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210515151403.png" /> | <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210515152538.png" style="" /> |

其中，在等号左边的变量称为 ***basic varibales*** 或者是 ***slack variables***  , 等号右边的变量称为  ***nonbasic variables*** .



## Dual

对于一个标准型的 LP 问题实例：
$$
\begin{aligned}
& \text{ maximize } & \textbf{c}^T \textbf{x} \\
& \text{ subject to } & \\
& & \textbf{Ax} \le \textbf{b} \\
& & \textbf{x} \ge 0
\end{aligned}
$$
其对偶问题为：
$$
\begin{aligned}
& \text{ minimize } & \textbf{b}^T \textbf{y} \\
& \text{ subject to } & \\
& & \textbf{A}^T \textbf{y} \ge \textbf{c} \\
& & \textbf{y} \ge 0
\end{aligned}
$$
前者常在教材中称为 Primal LP，后者称为 Dual LP 。

一个简单的例子：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210515152824.png" style="width:70%;" />

在该例子当中：
$$
\begin{aligned}
\textbf{x} &= [x_1, x_2]^T \\
\textbf{c} &= [1, 6]^T  \\
\textbf{b} &= [200, 300, 400]^T \\
\textbf{A} &=
\begin{bmatrix}
1 & 0 \\
0 & 1 \\
1 & 1 \\
\end{bmatrix}
\end{aligned}
$$
而在 Dual LP 当中：$\textbf{y} = [y_1, y_2, y_3] ^ T$ .

下面来证明关于 Dual Problem 的 3 个性质：

- Weak Duality
- Strong Duality
- Complementary Slackness



### Weak Duality

我们知道最大流-最小割也是对偶问题，它们的 Weak Duality 是这样的：

> Let $f$ be any flow and $(A, B)$ be any cut. Then $val(f) \le cap(A, B)$ .

在 LP 问题中，同样具有这一性质：

> 设 $\textbf{x}, \textbf{y}$ 分别是标准型 Primal LP 和 Dual LP 的一个可行解，那么有 $\textbf{c}^T \textbf{x} \le \textbf{b}^T \textbf{y}$ .
>
> 证明：
>
> - 由于 $\textbf{A}^T \textbf{y} \ge \textbf{c}$ ，那么 $\textbf{x}^T \textbf{A}^T \textbf{y} \ge \textbf{x}^T \textbf{c} = \textbf{c}^T \textbf{x}$ （三者都是 $1 \times 1$ 的矩阵）。
>
> - 同理，$\textbf{A} \textbf{x} \le \textbf{b}$ ，那么 $\textbf{y}^T \textbf{A} \textbf{x} \le \textbf{y}^T \textbf{b} = \textbf{b}^T \textbf{y} $ 。那么就有：
>   $$
>   \textbf{c}^T \textbf{x} = \textbf{x}^T \textbf{c} \le \textbf{x}^T \textbf{A}^T \textbf{y} = \textbf{y}^T \textbf{A} \textbf{x} \le \textbf{y}^T \textbf{b} = \textbf{b}^T \textbf{y}
>   $$



### Strong Duality

LP 问题的强对偶性如下：

> 如果 Primal LP 存在最优解 $\textbf{x}_0$ ，那么 Dual LP 也必然存在最优解 $\textbf{y}_0$ ，且目标函数值 $\textbf{c}^T \textbf{x}_0 = \textbf{b}^T \textbf{y}_0$ ，反之亦然。
>
> 通过证明 Complementary Slackness 可得。



### Complementary Slackness

Complementary Slackness 描述的是：

- Primal LP 的变量与 Dual LP 的约束之间的关系。
- Dual LP 的变量与 Primal LP 的约束之间的关系。

Complementary Slackness 的数学描述，个人觉得是 Refs [4] 的描述是最好理解的（这里不建议看 Wikipedia ），而证明过程最简洁的是 Refs [5] 。

> **Theorem** *(Complementary Slackness)* 
>
> Let $\textbf{x}$ be a feasible solution to the primal LP and $\textbf{y}$ be a feasible solution to the dual LP. Then $\textbf{x}$ is optimal to the primal and $\textbf{y}$ is optimal to the dual **if and only if** the conditions of *Complementary Slackness* hold:
> $$
> (b_i - \sum_{j=1}^{n}a_{ij}x_j)y_i = x_{n+i}y_i = 0 \text{ for } i=1,2, \dots, m
> $$
> and
> $$
> (\sum_{i=1}^{m}a_{ji}y_i - c_j)x_j = y_{m+j}x_j = 0 \text{ for } j=1,2, \dots, n .
> $$
> **Proof**
>
> - $\textbf{x}$ and $\textbf{y}$ are both optimal for their respective LPs
>   $\iff$ (by weak duality)
> - $\textbf{c}^T \textbf{x} = \textbf{b}^T \textbf{y}$ 
>   $\iff$ (since $\textbf{b}^T \textbf{y}$ is $1 \times 1$, and recall $\textbf{c}^T \textbf{x} \le \textbf{y}^T \textbf{Ax} \le \textbf{y}^T \textbf{b}$)
> - $\textbf{c}^T \textbf{x} = \textbf{y}^T \textbf{Ax}$ and $\textbf{y}^T \textbf{Ax} = \textbf{y}^T \textbf{b}$ 
>   $\iff$ (Algebra)
> - $(\textbf{A}^T \textbf{y} - \textbf{c})^T \textbf{x} = 0$ and $\textbf{y}^T (\textbf{Ax} - \textbf{b}) = 0$ .
>   $\iff$
> - The complementary slackness conditions hold.

从 LP 问题的 Slack 形式来看待这个性质：

- 如果 Primal 的第 i 个 Slack Variable 不为 0 ( i.e. $x_{n+i} > 0$ ) ，那么 Dual 的第 i 个 Non-basic Variable 等于 0 ( i.e. $y_i = 0$ ) .
- 如果 Dual 的第 j 个 Slack Variable 不为 0 ( i.e. $y_{m+j} > 0$ ) ，那么 Primal 的第 j 个 Non-basic Variable 等于 0 ( i.e. $x_j = 0$ ) .

如果把 Primal 的变量 $x_i$ 看作是一个资源 (Resource) 的数量，那么 Dual 的变量 $y_j$ 就是资源的价值 (Value)，Complementary Slackness 揭示了一个十分简单的经济学原理：

- $x_{n+i} > 0$ 表示资源数量存在冗余，那么有 $y_i = 0$ 表示资源的冗余部分是没有价值的。
- $y_{m+j} > 0$ 表示资源仍然具有价值（价格不为 0 ），那么有 $x_j = 0$ 表示该资源供应短缺。



TODO: 加一个凸优化问题的 Topic 

## References

- [1] DPV - Algorithm
- **[2] CLRS - Introduction to Algorithm (Chapter 29)**
- [3] [Wikipedia - Linear Programming](https://en.wikipedia.org/wiki/Linear_programming)
- [4] https://www.math.ubc.ca/~anstee/math340/340complementaryslackness.pdf
- [5] https://www.matem.unam.mx/~omar/math340/comp-slack.html

