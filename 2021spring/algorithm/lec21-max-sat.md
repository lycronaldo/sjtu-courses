## Maximum Satisfiability

主要内容：

- Maximum Satisfiability (MAX-SAT) 问题
- Flipping a Coin - 解决 MAX-SAT 的一个随机算法，近似因子为 1/2





## Introduction

![](https://gitee.com/sinkinben/pic-go/raw/master/img/20210605170154.png)

- 即：给定一个 SAT 的实例，求一组 $x_i$ ，使得结果为 true 的那些子句 $C_j$ 的权值之和最大。
- 如果子句的权值均为 1，那么 MAX-SAT 就是使得尽可能多的子句为真。

## Flipping a Coin

这个随机算法很牛，我再一次大受震撼：对于每一个 $x_i$ ，以 $0.5$ 的概率给它赋值为 true 。

可以证明，这是一个 1/2-approximation algorithm .

> **Proof**
>
> - Let $W$ be a random variable that is equal to the total weight of the satisfied clauses.
> - Define an indicator random variable $Y_j$ for each clause $C_j$ such that $Y_j = 1$ if and only if $C_j$ is satisfied. Then
>
> $$
> W = \sum_{j=1}^{m} w_j Y_j
> $$
>
> - Use $OPT$ to denote the value of optimal solution, then
>
> $$
> \textbf{E}[W] = \sum_{j=1}^{m}w_j \textbf{E}[Y_j] = \sum_{j=1}^{m}w_j \textbf{Pr}[C_j \text{ is staisfied}]
> $$
>
> - Since each variable is set to `true` independently, we have
>
> $$
> \textbf{Pr}[C_j \text{ is satisfied}] = 1 - (\frac{1}{2})^{l_j} \ge \frac{1}{2}
> $$
>
> where $l_j$ is the number of literals in clause $C_j$. Hence we have:
> $$
> \textbf{E}[W] \ge \frac{1}{2} \sum_{j=1}^{m}w_j \ge \frac{1}{2}OPT
> $$

从上面的分析可以看出：

- 如果我们每个子句的长度至少是 $k$ ，那么 $l_j \ge k$ ，那么上述算法就是一个 $（1 - (\frac{1}{2})^k \textit{- approximation algorithm}$  。
- 这一随机算法作用在具有长子句的 MAX-SAT 是比较有效的。



## References

- [1] Vaz04 - Approximation Algorithms (Chapter 16)



