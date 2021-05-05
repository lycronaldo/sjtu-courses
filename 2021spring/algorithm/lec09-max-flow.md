## Max Flow

重点内容：Network Flow 问题会出证明题。

本节的内容是最大流 (Max Flow) - 最小割 (Mini-Cut) 问题，涉及的算法是 Ford-Fulkerson 算法。



## Definitions

**Flow Network**

流网络 (Flow Network) ，即图 $G = (V,E,s,t,c)$ :

- $s$ 是源点，$t$ 是终点。
- $\forall{e \in E}, c(e) > 0$ ，边的权值 $c(e)$ 表示该边的最大流量。

Max Flow 问题：找到给定的 $G$ 中，$s$ 到 $t$ 的最大流量。例如在下图中，最大流量为 30 （在点 $u$ ，20 的流量分为 10 + 10 走 2 个不同的边）。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421185748.png"/>

定义 $st-flow$ 函数 $f$ ，$f(e)$ 表示实际流过该边的流量。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210422133219.png" style="width:67%;" />



**Minimum Cut**

严格来说是最小的 $st-cut$ ，即：给定 $s,t \in V$ ，求出权值最小的割集（即边割集）$S$ ，使得图分为 $A,B$ 两部分，且 $s \in A, t \in B$ .

显然，$S$ 中的所有边，一端在 $A$ 中，另外一端在 $B$ 中。 

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421185609.png" style="width:60%;" />



最大流-最小割已被证明是对偶问题，因此求出最大流即求出最小割。



## Greedy Algorithm

Greedy Algorithm:

- Start with $f(e)=0$ for each edge $e \in E$.
- Find a $s \rightarrow t$ path $P$ where each edge has $f(e) < c(e)$.
- Augment flow along path $P$.
- Repeat until you get stuck.

通过一个例子说明贪心策略是如何工作的。

1. 给定的图如下。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421192140.png" style="width:67%;" />

2. 从 $f(e) = 0$ 的边开始找路径。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421192227.png" style="width:67%;" />

3. 修改每个边的实际流量为该路径上的边的最大权值

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421192322.png" style="width:67%;" />

4. 从另外的 $f(e) = 0$ 的边开始找路径，重复上述过程。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421192429.png" style="width:67%;" />

5. 再重复一遍。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421192544.png" style="width:67%;" />

6. 最终结果。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421192619.png" style="width:67%;" />

上述贪心算法得到的最大流为 16 ，而实际上，我们的最大流其实是可以达到 19 。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421192731.png"  style="width:67%;" />

贪心算法得不到最优的原因：**Once greedy algorithm increases flow on an edge, it never decreases it.** And we need some mechanism to undo a bad decision.

比如上面的 `2/2` 这条边，相当于「走错路」了，但是贪心算法无法消除这种错误，这是与 Ford-Fulkerson 算法的本质区别。



## Ford-Fulkerson Algorithm

**Residual Network**

大白话：在另外一个图中，构造一个额外的反向边，权值是实际流过原边的流量 $f(e)$ 。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421193242.png" style="width:75%;" />



**Augmenting Path**

不少地方把 Augmenting Path 翻译为「增广路径」，注意与二分图中的增广路径区分。

残留网络 $G_f$ 具有下列性质：

- An augmenting path $P$ is a simple $s \rightarrow t$ path in the residual network $G_f$ .
- The bottleneck capacity of an augmenting path $P$ is the minimum residual capacity of any edge in $P$.



**Ford-Fulkerson**

该算法的时间复杂度为 $O(VEC)$ ，经过优化之后可以达到 $O(E^2\log{C})$ .

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421204043.png" style="width:67%;" />



**Augment**

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421204119.png" style="width:67%;" />

**Example**

来自 CLRS 的《算法导论》的一个例子。

注意，在图 e 和 图 f 中，$(s, v_2)$ 之间的 2 条边的权值标注应该是反过来的（这应该是书本的谬误）。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421205939.png" style="width:80%;" />

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210421210410.png" style="width:80%">

观察从 $t$ 出来的 2 个反向边，根据定义，这 2 条边的权值就是流入 $t$ 的流量，因此此处例子的最大流为 23 。

前面提到，贪心算法失效的原因是：一旦选定了一条「错误」的边（即不在最优解中的边），贪心策略无法消除这种错误。而这里的 Ford-Fulkerson 算法是怎么做到这一点的呢？

在图 (a) 中，原图 $G$ 给定的一边为 $v_2 \rightarrow v_1$ ，在算法执行过程中，在 Residual Network 中，会产生一个反向边 $v_1 \rightarrow v_2$ ：

- 在图 (b) 中，选择了一个 Augmenting Path，包括了 $v_2 \rightarrow v_1$ 这条边，执行 $Augment(f,c,P)$ 之后，产生反向边  $v_1 \rightarrow v_2$ 。
- 在图 (c) 中，选择了一个 Augmenting Path，包括了反向边  $v_1 \rightarrow v_2$ ，这表明在之前的步骤中，流量流经  $v_2 \rightarrow v_1$ 是错误的选择，这就是 Ford-Fulkerson 「错误修正」的方式。

引用自 Refs [4] 的一个图解：

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210422133759.png" style="width:67%;" />

如果只是学这个算法去做题的话，看到这里已经可以了。

目前为止，我们已经解决了最大流问题，那么对应的最小割是什么呢？在原图 (a) 中，$\{(v_1, v_3), (v_4, v_3), (v_4, t)\}$ 是最小割，因为在最大流 (f) 中，这三个边的流量都是「满」的。即：在最大流中，$f(e) = c(e)$ 的边就是最小割的边。

这一（最小）割集意味着 2 点：

- 它使得 $s$ 和 $t$ 不再连通 。
- 把图分为 $B=\{v_3,t\}$ 和 $A=\{s, v_1, v_2, v_4\}$ 两部分。从图中删除这一割集后，$s$ 能到达 $A$ 中的任意一点，$B$ 中的任意一点都能到达 $t$ 。



## Max-Flow Min-Cut Theorem

**Lemma** Let $f$ be any flow and let $(A, B)$ be any cut. Then, the value of the flow $f$ equals the network flow across the cut $(A, B)$.
$$
val(f) = \sum_{\text{out of } A} f(e) - \sum_{\text{in to } A} f(e)
$$
<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210422132457.png" style="width:75%;" />

设把最大流的起点和终点分别为 $s,t$ , 并且 $s \in A, t \in B$ ，那么有:
$$
\begin{aligned}
val(f) &= \sum_{\text{out of } s} f(e) - \sum_{\text{in to } s} f(e) \\
       &= \sum_{v \in A} (\sum_{\text{out of } v} f(e) - \sum_{\text{in to } v} f(e) ) \\
       &= \sum_{\text{out of } A} f(e) - \sum_{\text{in to } A} f(e)
\end{aligned}
$$

第一个等号是根据 $st-flow$ 函数 $f$ 的定义；第二个等号 Flow Network 的性质；第三个等号根据集合的定义。



**Theorem - Weak Duality**

证明最大流-最小割的弱对偶性。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210422134345.png" style="width:75%;" />

**Corollary**

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210422141417.png"  style="width:75%;" />



**Max-Flow Min-Cut Theorem**

最大流 / 最小割的 3 个等价条件。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210422143403.png" style="width:80%;" />

- [$\text{i} \Rightarrow \text{ii}$] : 根据上述的 Weak Duality 性质和 Corollary 。

- [$\text{ii} \Rightarrow \text{iii}$] : 证明其逆否命题，即 $\lnot \text{iii} \Rightarrow \lnot\text{ii}$ .

  - 假设 $G_f$ 中存在一个 Augmenting Path $P$ ，那么在路径 $P$ 上，我们还能继续向 $G_f$ 增加流量，这就意味着 $f$ 不是一个 Max Flow 。证明成立。

- [$\text{iii} \Rightarrow \text{i}$] 

  - Let $f$ be a flow with no augmenting paths.
  - Let $A$ be set of nodes reachable from $s$ in residual network $G_f$ . And $B$ is the set containing the left nodes (and every node in $B$ can reach $t$ ).
  - By definition of $A$ : $s \in A$ .
  - By definition of flow $f$ : $t \notin A$ .
  - 如图所示，$G_f$ 中没有 Augmenting Path，当且仅当，在**原图** $G$ 中：
    - 所有 $A \rightarrow B$ 的边，$f(e) = c(e)$ 
    - 所有 $B \rightarrow A$ 的边，$f(e) = 0$
    - 为什么呢？如果 $G$ 存在 $A \rightarrow B$ 的边 $e$ ，$f(e) < c(e)$，那么在 $G_f$ 中会存在 $A \rightarrow B$ 的边，权值为 $c(e) - f(e)$，使得 $G_f$ 存在 Augmenting Path 。同理， $G$ 所有 $B \rightarrow A$ 的边 $f(e) = 0$，意味着它在 $G_f$ 中 对应的 $e^{\text{reverse}}$ （从 A 到 B 到边）的权值均为 0 ，同样保证了 A 不能到达 B （即 $G_f$ 没有 Augmenting Path）。
  - 那么就有：

  $$
  \begin{aligned}
  val(f) &= \sum_{e \text{ out of } A} f(e) - \sum_{e \text{ in to } A} f(e) \\
         &=  \sum_{e \text{ out of } A} f(e) - 0 \\
         &= \text{cap(A, B)}
  \end{aligned}
  $$

  

  <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210422145715.png" style="width:50%" />

  

  

## Further Analysis

复杂度分析的大致想法是：每次找到路径 $P$，都会使得流量至少增加 1 个单位，但最大流量只能为 $|V|C$ （所以最多只能有 $|V|C$ 个 Augmenting Path），$C$ 是图中边的最大流量（边权最大值）。

每次通过 BFS/DFS 找到一个 Augmenting Path，需要在 $O(E)$ 时间，因此总的时间复杂度为 $O(VEC)$ ，因此该算法是一个伪多项式算法（与背包问题类似）。而在实际计算机的运行当中，$C$ 是通过比特位来存储的，因此算法复杂度存在一定程度的「指数」成分。

考虑如下图的情况（图来自 Refs[2] ），如果每次找到的 Augmenting Path 是以下 2 条路径的交替：

- $P_1: s \rightarrow u \rightarrow v \rightarrow t$
- $P_2: s \rightarrow v \rightarrow u \rightarrow t$

显然，每次网络流的容量只增加 1 ，Ford-Fulkerson 算法需要进行 200 次迭代。

针对这一特殊情况，后续还要对该算法进行改进，采用更为「明智」的策略去选取 Augmenting Path:

- 选取 bottleneck capacity 较大（或者最大）的路径
- 选取边数最少的路径

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424195817.png" style="width:57%;" />



此外，在该算法当中还有一个重要的问题：Ford-Fulkerson 算法假定图 $G$ 的边权值都是整数，并且每个边的流量也是整数，在这种假设条件该算法可以找到图 $G$ 的最大流。如果边权值和流量可以为浮点数，那么 Ford-Fulkerson 算法还能正常工作吗？

**Ford-Fulkerson 算法只适用于边权值为有理数的情况。**有理数包括整数和分数，如果为分数，可以对所有权值乘以分母的最小公倍数，转换为整数处理。

为什么无理数不适用呢？可以参考这个 [Slide](https://www.cs.princeton.edu/~wayne/kleinberg-tardos/pdf/07DemoFordFulkerson.pdf) 给出的例子。



## References

- [1] https://www.cs.princeton.edu/courses/archive/spr04/cos226/lectures/maxflow.4up.pdf
- [2] KT05 - Algorithm Design (Chapter 7)
- [3] CLRS - Introduction to Algorithm
- [4] https://oi-wiki.org/graph/flow/max-flow/

