## Lecture 02 - Stable Matching

Date: 2021/03/24

这节课的主要内容是匹配问题，因为内容过于晦涩，所以就不用英文写了。

## 问题描述

给定集合 $M = \{m_1,...,m_n\}$ 和 $W = \{w_1, ..., w_n\}$, 并且 $|M| = |W| = n$ ，如何找到一个 *稳定匹配* $S$ ?

首先需要区分几个概念。

**匹配**

一个匹配 $S$ 是 $M$ 和 $W$ 的笛卡尔积 $M \times W$ 的子集，并且：

- 任意的 $m \in M$ ，在 $S$ 中最多出现一次。
- 任意的 $w \in W$ ，在  $S$ 中最多出现一次。



**完美匹配**

一个匹配 $S$ 当且仅当满足 $|S| = |M| = |W| = n$ ，那么匹配 $S$ 是完美匹配。

**不稳定对 (Unstable Pair)**

如果存在这样的喜好排序：

```
m  : w' > w
w' : m  > m'
```

不管 $m'$ 和 $w$ 的喜好排序如何，给定匹配集合是 $S = \{(m, w), (m', w')\}$ ，也就是说，我们「强行」让 $m, w$ 都与次一级别的元素匹配，那么这时候说，$pair = (m, w')$ 相对于匹配集 $S$ 来说是不稳定的，$S$ 是一个非稳定匹配。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210324211419.png" style="width:30%"/>

**稳定匹配**

设 $U = M \times W$，匹配集合 $S$ 是一个稳定匹配，当且仅当 $\forall p \in (U - S)$ , $p$ 相对于 $S$ 来说是稳定的。



## Gale-Shapley 算法

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210324212632.png" style="width:80%;" />

如何证明其输出的匹配 $S$ 是一个稳定匹配？