## Lec07 - Reduction

Refs:
- [1] [Algorithms - DPV](http://algorithmics.lsi.upc.edu/docs/Dasgupta-Papadimitriou-Vazirani.pdf) 

**Notes** - 我不知道 "reduce / reduction" 的中文翻译是「规约」还是「归约」，但行文如果一直用 "reduce" 这个词，文风就很奇怪，所以这里使用「规约」一词作为中文翻译，因为我的苹果输入法已经记住这个词了。

在前面的 Lecture 中提到，SAT 已被证明为是一个 NPC 问题，这就意味着：任何一个 NP 问题都能规约为 SAT 问题。但解决 SAT 问题依旧是十分困难的，因此有必要对 SAT 进一步规约。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210416192117.png" style="width:70%;" />

这节课的主要内容就是上图的几个例子，「简单」介绍一下规约的 trick:

- Rudrata path => Rudrata cycle
- 3-SAT => Independent Set
- SET => 3-SAT







## Rudrata path => Rudrata cycle

关于 Rudrata cycle 问题，可以参考 `Lec05.md` .

如图所示，假设问题是要在 $G(V,E)$ 中寻找一个 Rudrata path ，那么可以构造 $G'(V', E')$ ，并且令 $V'=V \cup \{x\}$ ，$ E' = E \cup \{(x,s), (x,t)\}$  . 那么，如果 $G'$ 存在 Rudrata cycle ，我们把 $x$ 从 $G'$ 中去除，即可解决在 $G$ 中寻找 Rudrata path 的问题。当然，把 Rudrata cycle 规约为 Rudrata path 也是可以的。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210416192757.png" style="width:80%;" />



## 3-SAT => Independent Set

SAT 和独立集的相关概念参考 `Lec05.md` 。

**3-SAT **

给定一个 3-SAT 的 CNF 表达式，寻找一组变量 $x_i$ 的赋值，使得该 CNF 为真。以下为一个 3-SAT 表达式例子：
$$
(\overline{x} \lor y \lor \overline{z}) \land
({x} \lor \overline{y} \lor {z}) \land
({x} \lor {y} \lor {z}) \land
(\overline{x} \lor \overline{y})
$$
**Independent Set**

给定图 $G(V,E)$ 和整数 $g$ ，是否存在一个包含 $g$ 个顶点的独立集 $S$ 。

下面阐述 3-SAT 规约为 Independent Set 的方法。

- 对于 3-SAT 的 CNF 表达式，其每个子句 (Clause) 的长度都小于等于 3 ，即最多只包含 3 个文字 (Literal) 。只需要从 3 个 Literal 中任选一个（赋值为 True），那么该子句即可为 True。每个子句用图来表示，就是一个三角形。
- 如果某个子句中选择了 $x$ ，那么在其他子句中就不能选择 $\overline{x}$ 。因此，把 $x$ 与位于其他三角形的 $\overline{x}$ 连接一条边。

以上述 CNF 为例，最终得到的图 $G$ 如下：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210416201951.png" style="width:80%;" />

显然，3-SAT 的解就是在图 $G$ 中寻找一个大小为 $g$ 的独立集 $S$ 。$g$ 是三角形的个数，也是 3-SAT 的 CNF 表达式的子句个数，此处 $g=4$ 。最终：

- 如果 $S$ 包含了 $x$ ，说明 CNF 中 $x$ 应为 True ；
- 如果 $S$ 包含了 $\overline{x}$ ，说明 CNF 中 $x$ 应为 False ；
- 如果 $S$ 二者均不包含，说明 CNF 的真值与 $x$ 无关，$x$ 可任意赋值。

此处的逆命题也是成立的，即：如果已知一组变量 $X$ 的赋值，它能够使得 3-SAT 的 CNF 表达式为真，那么 CNF 对应的图 $G$ 具有独立集 $S$ 。$S$ 的大小是 $X$ 中**有唯一赋值**的变量个数（因为存在某些变量，赋值 T/F 均不影响 CNF 的真值）。



## SAT => 3-SAT

正如 Refs [1] 里面所述：

> This is an interesting and common kind of reduction, from a problem to a *special case* of itself.

显然，合取符号 $\land$ 是具有分配律的，即：
$$
k \land (a \lor b) = (k \land a) \lor (k \land b)
$$
通过真值表易证。

对于任意 SAT 的一个子句，我们设为：$C = (a_1 \lor a_2 \lor \dots \lor a_k)$ ，其中 $k \ge 3 $  。我们有以下的等价变换：
$$
C' = (a_1 \lor a_2 \lor y_1) \land
(\overline{y_1} \lor a_3 \lor y_2) \land
(\overline{y_2} \lor a_4 \lor y_3) \land \dots \land
(\overline{y}_{k-3} \or a_{k-1} \lor a_k)
$$
 其中，$y_i$ 是新增的变量（下面说明其作用），在给定一组变量 $a_i$ 的条件下，可以证明存在一组 $y_i$ 使得 $C = C'$ : 

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210416212600.png" style="width:70%;" />

- 证明 1 ：若 $C'$ 为真 ，那么至少有一个 $a_i$ 为真（即 $C$ 为真）。
  - 反证法证明。如果 $a_i$ 均为假，那么从 $C'$ 第 1 项看， $y_1$ 必须为真；那么可推出第 2 项中 $y_2$ 为真；以此类推，在倒数第二项中，$y_{k-3}$ 为真，最后一项 $(\overline{y}_{k-3} \lor a_{k-1} \lor a_{k})$ 必然为假，导致 $C'$ 为假，与已知条件矛盾。
- 证明 2 ：若 $C$ 为真，那么 $C'$ 为真。
  - $C$ 为真，说明至少有一个 $a_i$ 为真。在 $C'$ 中，令与这样的 $a_i$ 搭配的那些 $y$ 为假（不带帽子的 $y$），即可使得 $C'$ 为真。

显然，对于任意的 SAT 表达式，对于它的**长度大于 3 的子句**，都进行如上的变换，最终可以得到一个 3-SAT 表达式。



## Summary

Reduce 这样一个过程，从英文单词的含义上理解，理应是把一个问题 $P$ 简化为 $P'$ ，使得它更容易解决。

然而往往并非如此，比如 NPC 问题是所有 NP 问题的 Reduction 的结果，这里的 "Reduction" 试图把多个问题「汇总」为一个问题 $P'$ ，这样的本意是：只要我们能解决 $P'$ ，那么我们就能解决 Reduction 前的所有 NP 问题。

因此，从这个角度来看，Reduction 并没有把问题的难度简化，反而有可能增加其困难程度。也就是说，$P$ 经过 Reduction 得到的 $P'$，其难度至少是**「大于等于」**$P$ 的。

