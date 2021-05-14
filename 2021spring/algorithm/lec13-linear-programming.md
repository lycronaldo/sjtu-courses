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
> By simple geometry — think of the profit line passing through this vertex. Since all the vertex’s neighbors lie below the line, the rest of the feasible polygon must also lie below this line.

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

参考 CLRS：TODO。



## Duality

参考 DPV：TODO。



## References

- [1] DPV - Algorithm
- [2] CLRS - Introduction to Algorithm

