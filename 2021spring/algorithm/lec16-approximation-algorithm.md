## Approximation Algorithm

主要内容：

- 组合优化问题 (Combinatorial Optimization)
- 近似算法 (Approximation Algorithms)

Stop at slide: hand17 - P35.





## Introduction





## Examples

这部分包括：

- Vertex Cover
- Set Cover



### Vertex Cover

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210528204600.png" style="width:80%;" />

对于 Cardinality Vertex Cover ，一个近似算法为：

> Find a maximal matching in $G$ and output the set of matched vertices.
>
> <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210530194449.png" style="width:67%;" />

这是一个近似因子 (Approximation Factor) 为 2 的近似算法：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210530194707.png" style="width:67%;" />

对于一个完全二分图 $K_{n,n}$ ：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210530195836.png" />

上述算法得到的 $|M| = n$ ，即点覆盖的近似结果为 $2n$ （但点覆盖的最优结果是 $n$），这就是达到了近似算法的 Lower Bound.

PS: 在任意二分图中，最大匹配的基数等于最小点覆盖的基数。

### Set Cover

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210528210132.png" style="width:80%;" />

一个最简单的近似算法是**贪心**。每次选取花费最小的 $S_i$ ，然后从 $U$ 中去除 $S_i$ 的所有元素，重复这个过程直到 $U$ 是空集。

如果每个 $S_i$ 的花费都是 1 ，那么问题就称为 “Cardinality Set Cover” 。比如：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210530202511.png" style="width:67%;" />

- 对于每个点 $x$ ，在它 30 miles 内的点都和它相邻，用集合 $S_x$ 表示与 $x$ 相邻的点。
- 那么问题转换为：怎么选择 $S_x$ 使得所有的点都被覆盖？



