## Extension on Ford-Fulkerson Algorithm

上一节提到，影响 Ford-Fulkerson 算法的效率的最主要因素是 Augmenting Path 的选取，本节的内容是针对这一点对算法进行改进，改进后的时间复杂度为 $O(E^2 \log{C})$ .

主要内容：

- Capacity-Scaling Algorithm
- Shortest Augmenting Paths
- Dinitz’ Algorithm



## Capacity-Scaling Algorithm

### Overview

Choosing augmented paths with large bottleneck capacity.

- Maintain scaling parameter $\Delta$ .
- Let $G_f(\Delta)$ be the part of the residual network containing only those edges with capacity $\ge \Delta$ .
- Then any augmenting path in $G_f(\Delta)$ has bottleneck capacity $\ge \Delta$ .

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424203029.png" style="width:50%;" />

Pseudo code of Capacity-Scaling algorithm:

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424203141.png" style="width:60%;" />

### Correctness

**Assumption**: All edge capacities are integers between 1 and $C$.

There are two observations:

- **Invariant**: The scaling parameter $\Delta$ is a power of 2.
- **Integrality invariant**: Throughout the algorithm, every edge flow $f(e)$ and residual capacity $c_f(e)$ is an integer.



We need to prove that: **If capacity-scaling algorithm terminates, then $f$ is a max flow.**

**Proof**

- By integrality invariant, when $\Delta = 1$, then $G_f(\Delta) = G_f$
- Upon termination of $\Delta = 1$ phase, there are no augmenting paths.
- According to the *Augmenting Path Theorem*, $f$ is a max-flow if and only if there is no augmenting path. ( See `lec09.md`)



### Analysis

There are 3 lemmas:

- Lemma-1: There are at most $1+\lceil\log_{2}{C} \rceil$ iterations of outer while loop.
  - Proof. It's obvious, since initially $C/2 < \Delta \le C$ and $\Delta$ decreases by a factor of 2 in each iteration.
- Lemma-2: There are $\le 2|E|$ augmentations per scaling phase.
- Lemma-3: Let $f$ be the flow at the end of a $\Delta$-scaling phase. Then, the max-flow value $\le \text{val}(f) + |E|\Delta$ .



We need to prove that: **This algorithm takes $O(E^2 \log{C})$ time.**

**Proof**

- Lemma-1 + Lemma-2 $\Rightarrow$ $O(E\log{C})$ .
- Finding an augmenting path takes $O(E)$ time.
- Hence it takes time $O(E^2 \log{C})$ .





## Shortest Augmenting Path

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210424210802.png" style="width:67%;" />



## Dinitz' Algorithm

