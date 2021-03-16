## Eventual Consistency

ADS Course, lecture-03.

Disadvantages of Sequential Consistency / Release Consistency (SC/RC):

- Very slow
  - Must **ask** before each operation
  - IVY (SC-System): ask master node
  - TreadMarks (RC-System): ask lock server
- Require highly available connections
  - There are lots of chatter request-response between master/worker



**Strong Consistency**

Strong Consistency，也就是强一致性。传统的关系型数据库是天然的强一致性：当一条数据发生添加、更新操作时，强一致性保证，在操作完成之后，所有**之后**查询该条记录的请求都会拿到最新的数据。由于大部分程序员都是从关系型数据库的时代成长起来，因此大家下意识中会把 Strong Consistency 作为理所应当的事情，但是关系型数据库的 Strong Consistency 是依赖数据加锁实现的，牺牲了系统的整体性能。

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210316165937.png"  style="width:67%;" />

如上图所示，如果该分布式存储系统中有三个节点, 如果 update 操作发生在 node A, 当操作完毕时，node A 需要把最新信息 replicate 到 node B/C, 这时为了保证 strong consistency, 所有在 node B/C 读取该条记录的请求都会被 block, 直到该条数据完整的从 node A replicate 完毕。



**Eventual Consistency**

个人理解，Eventual Consistency 保证当一个 update/insert 操作发生后，所有的 `read` **最终**都会读取到操作发生后的值，它不像 Strong Consistency 那样保证「马上」能读取到最新的值，Eventual Consistency 可能需要 `read` 多次才能读取到新值。

假设我们基于 Eventual Consistency 去设计一个分布式存储系统，以上述场景为例，假设节点 A 已完成数据 `x` 更新，但在短时间内，数据 replicate 还没完成。在 B 和 C 中，它们对于 `read(x)` 的处理方式不是阻塞，而是返回旧值。当用户发现数据 `x` 的异常，会重新发起多次 `read` ，最终还是会得到新值。

我觉得这个 Eventual Consistency 好无聊 😅😅😅😅 。



**Casual/Casual+ Consistency**

Casual/Casual+ Consistency 属于 Eventual Consistency 中一种，比 Sequential Consistency 的约束要弱一些，它只对事务的因果关系有要求（而 Sequential Consistency 还对事务的执行结果有要求）。

因果一致性主要用于跨地域的分布式存储系统，比如一些大型的社交网站的数据库，数据中心分布世界各地，用户访问的往往是离他们最近的数据中心，数据中心之间会并行地进行双向的数据同步（降低用户的感知延迟）。

出自论文： 

- [Don’t Settle for Eventual: Scalable Causal Consistency for Wide-Area Storage with COPS](https://ipads.se.sjtu.edu.cn/courses/ads/paper/cops-sosp2011.pdf)
- [Causal memory: definitions, implementation, and programming](https://link.springer.com/article/10.1007/BF01784241)

论文阅读笔记：[Lec03-paper](./lec03-paper.md) .

Casual+ Consistency 是在 Casual Consistency 的基础上，加入了收敛冲突处理 (Convergent conflict handling) .

> 通过 2 个属性来定义 *Causal+ Consistency*：
>
> - *Causal consistency* requires that values returned from `get` operations at a replica are consistent with the order defined by $\leadsto$ (causality)
>   - 直观的理解，对于某个数据 `x` 的操作，所有的 `put` 操作都必须要在 `get` 之前出现。
>   - *Causal consistency* 就是说 `get`操作读到的值，和之前 $\leadsto$ 顺序的操作得到的结果，两者应当一致。
>   - 冲突不受因果一致性的影响，使得数据副本永远存在差异。比如，`put(x, 1)`和 `put(x, 2)` 这 2 个操作，即使在存在因果一致性的约束条件，仍然允许一个副本返回 1 ，而另外一个副本返回 2 .
>   - 因此需要 Causal+ 去处理冲突
>
> - *Convergent conflict handling* requires that all conflicting puts be handled in the same manner at all replicas, using a handler function h. 
>   - 所有冲突的 `put` 方法需要被一个函数 `h` 统一处理（「统一」意味着在所有的数据副本上，以相同的方式处理冲突）。
>   - 函数 `h` 需要满足 associative and commutative （应该是交换律和结合律），即：`h(a, h(b, c))` 和 `h(c, h(b, a))` 等价。
>   - 对于写冲突的处理，常用方法是 last-writer-wins rule ，只保留最后一次写的内容。
>   - COPS 可自定义冲突函数，不过默认是last-writer-wins。

下面举例说明没有 Causal Consistency 时，会发生些什么问题，以此说明 Causal Consistency 的作用。

**场景一**

- A 在朋友圈发表状态“我戒指丢了”
- A 在同一条状态下评论“我找到啦”
- B 在同一条状态下评论“太棒了”
- 远在美国的 C 看到「戒指丢了 -> 太棒了」，开始喷 B 
- C 刷新页面后看到「戒指丢了 -> 找到啦 -> 太棒了」，开启社死现场

在 Causal Consistency 的约束下，「太棒了」必须要在「我找到了」之后写入数据库。

**场景二**

- A 从好友中删除了 B
- A 发表了朋友圈 “清理了没用的好友”
- 远在美国的 B 看到了该朋友圈
- B 想去点个赞，发现点赞失败

在 Causal Consistency 的约束下，「向所有好友推送动态」应该在「删除好友」之后完成。



**References**

- [1] [浅谈分布式系统设计中的 Strong Consistency VS Eventual Consistency](https://cloud.tencent.com/developer/article/1102914)
- [2] [Eventually Consistent - Revisited](https://www.allthingsdistributed.com/2008/12/eventually_consistent.html)
- [3] [深入浅析一致性模型之 Causal Consistency](https://zhuanlan.zhihu.com/p/71913226)

