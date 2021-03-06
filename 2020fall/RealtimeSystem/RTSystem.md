## 实时系统与调度

考察方式：

+ Usual Tests: 10%
+ Projects: 60%
+ PRE: 30%

## Lecture-1 Introduction

+ **Real-time System Definition**: A system whose correctness depends not only on the **correctness** of logical results (e.g., decisions), but also on the **time** at which the results are produced.
+ 一些分类：
  + Real-time embedded: Nuclear reactor control, Flight Control, Mobile Phone, Drone (无人机)
  + Real-time but not embedded: Stock trading system, Skype, Video/Audio streaming and processing
  + Embbed but not real-time: Home temperature control, Blood pressure meter
+ **Characteristics of real-time systems:** timeless (及时性)
+ Classes of real-time system
  + **Hard real-time: **systems where it is absolutely imperative that responses occur within the required deadline. (飞行控制、原子核控制)
  + **Soft real-time: **systems where deadlines are important but which will still function correctly if deadlines are occasionally missed. (手机、网络对战游戏)
  + **Firm real-time: **systems which are soft real-time but in which there is no benefit from late delivery of service. (VR、电影)
+ 重要概念
  + **Task**: A code that implements one of the system functionalities
  + **Job**: a instance of a task
  + Task 的分类
    + **周期性** (Periodic): **Single rate** (all tasks have the same period); **Multi rate** (tasks have different periods)
    + **非周期性** (Aperiodic): Bounded arrival time (sporadic); Unbounded arrival time (event-based activation)
+ Type of deadline: hard, soft, firm
+ Relation between deadline and period: Implicit deadline（隐含的DDL）、Constrained deadline（强制的DDL）、Arbitrary deadline（任意的DDL）

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201009212806.png" alt="image-20201009212806179" style="width:67%;" />

+ Execution Model：抢占式（Preemptive）、非抢占式（Non-Preemptive）

## Lecture-2 

### Online Scheduling

+ 几个时间的定义：
  + $a_i$ : arrival time, 任务到达时间
  + $s_i$ : start time, 任务开始时间
  + $f_i$ : finish time, 任务完成时间
  + $R_i = f_i - a_i$ : Response time, 任务响应时间
  + $C_i = f_i - s_i$ : Compute time, 任务计算时间
  + $D_i$ : relative deadline
  + $d_i$ : absolute deadline
  + $slack_i = d_i - f_i$ : 任务提前完成
  + $lateness_i = L_i = f_i - d_i$ ：任务超时完成

|                           Figure 1                           |                           Figure 2                           |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201011170416.png"/> | <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201011170541.png" /> |
| <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201011171316.png" alt="image-20201011171316649" style="" /> | <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201011171402.png" alt="image-20201011171402931" style="" /> |

+ A real-time task $𝜏_i$ is said to be feasible iff $f_i \le d_i$ (or $R_i \le D_i$)
+ How to get a safe $c_i$ ? 重复测试运行时间，根据需要取平均值/最大值等。
+ “real time” 和 “real-time” 的区别？
  + When something happens in the current time, it is said to be real time. （用户感受不到系统的响应时间。）
  + When the response time of a system must be upper bounded by a given value, it is a “real-time” system.（任务在限定时间内完成，但是允许用户感受到系统延迟，即可以延迟知道这个任务完成了）
+ Schedule 的定义

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201011172943.png" alt="image-20201011172943812" style="width:70%;" />

+ 时间片与上下文切换

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201011173113.png" alt="image-20201011173113612" style="width:70%;" />

### Quantitative Analysis (Execution Time)

+ 最坏执行时间 WCET: Worst Case Execution Time
+ 最好执行时间 BCET: Bes Case Execution Time
+ 利用程序的 CFG (Control Flow Graph) 图进行分析 WCET
+ 影响执行时间的因素：循环上界（算法复杂度）、路径空间（Exponential Path Space）、无效路径（Infeasilble Paths）、处理器行为（Processor Behavior: Cache Effect）



## Lecture-3 Online Scheduling of Aperiodic Tasks (1)

+ 调度策略
  + 非抢占：先来先服务（First-in-first-out，FIFO）、时间片轮转（Round Robin）、短作业优先 SJF
  + 抢占：最早截止时间优先 EDF、固定优先级调度 FP、最短剩余时间优先 （Short Remaining Time First, SRTF）
+ PS：当 SRTF 不可抢占时，便退化为 SJF。
+ 要求总时延最低，怎么实现调度？
+ 调度算法的性质
  + Preemptive v.s. Non-preemptive
  + Work-conserving v.s. Non-work-conserving: 是否允许处理器有空闲时间
  + Static v.s. Dynamic
    + Static: scheduling decisions are taken based on fixed parameters, statically assigned to tasks before activation. e.g. FP、SJF
    + Dynamic: scheduling decisions are taken based on parameters that can change with time. e.g. SRTF
  + Offline v.s. Online
    + Offline: all scheduling decisions are taken before task activation: the schedule is stored in a table (**table- driven scheduling**).
    + Online: scheduling decisions are taken at run time on the set of active tasks.
  + Optimal v.s. Non-optimal
    + Optimal: They generate a schedule that minimizes a cost function, defined based on an optimality criterion (优化准则) .
    + Non-optimal: They generate a schedule according to a heuristic function that tries to satisfy an optimality criterion, but there is no guarantee of success.
    + 优化准则：保证可调度、最小化平均延迟时间 $Lateness$ 、最小化 DDL miss 数目。



## Lecture-4 Online scheduling of aperiodic tasks (2)

+ Proof: **If a task set is feasible, then EDF can also generate a feasible schedule for it.**
+ An online admission test for EDF

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201011190549.png" alt="image-20201011190548963" style="width:70%;" />

## Lecture-5 Online scheduling of periodic tasks

+ 一些重要记号
  + $\Delta$ : 时间线上的一个 slot，类似于小周期

+ **Agenda**
  + Scheduling algorithms for periodic real-time systems
  + RM schedulability tests (RM means Rate Monotonic, 速率不变的)

+ Periodic Tasks

|                          Definition                          |                          Assumption                          |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20201108180213.png) | ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20201108180247.png) |

+ 周期性任务的调度策略
  + Proportional share algorithm (等比例分配)
  + Cyclic scheduling (循环调度，a.k.a. timeline scheduling, table-driven scheduling)
  + Online scheduling policies
    + Fixed-priority policy: DM (Deadline Monotonic), RM (Rate Monotonic)
    + Dynamic policy: EDF

+ **Proportional Share Algorithm**

  把时间线分为若干个 slot，每个 slot 的长度为**所有任务周期的最大公约数**，即 $slot=GCD(t_1, \dots, t_n)$。在每个 slot 内，任务 $t_i$ 占据的时间长度为：$slot \times \frac{C_i}{T_i}$ .

  缺点：如果 slot 太小，容易造成巨大的 Context Switch开销。

|                                                              |                                                              |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20201108180853.png) | ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20201108180914.png) |

+ Cyclic Scheduling

  Examples: Air traffice control system, Space Shuttle, Boeing 777

  |                           具体做法                           |                            优缺点                            |
  | :----------------------------------------------------------: | :----------------------------------------------------------: |
  | <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201108195257.png" /> | ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20201108195412.png) |

+ Some Questions:
  + **What defines an optimal scheduling policy (in the sense of feasibility)?**
    ❌It can generate a feasible schedule even for infeasible task sets (seriously?!)
    ❌It always generates a feasible schedule
    ✅It generates a feasible schedule for a feasible task set
  + **Which policy is optimal for preemptive task sets?**
    ✅EDF
  + **Which policy is optimal for non-preemptive task sets?**
    ❌Non-preemptive EDF
    ❌FIFO
    ❌Non-preemptive fixed-priority
    ✅None of these
  
+ 固定优先级 Fixed-priority 之 RM 算法

  周期越小，优先级越高，任务运行速率 $rate = \frac{1}{T}$ .

  RM算法不是最优的（如下右图的例子）。但是，当 $\forall{i}, D_i = T_i$ 时，RM算法时所有**Fixed Priority**系列算法中的最优者。

  |                             定义                             |                             例子                             |
  | :----------------------------------------------------------: | :----------------------------------------------------------: |
  | <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201108200703.png" /> | ![](https://gitee.com/sinkinben/pic-go/raw/master/img/20201108201903.png) |

  

+ DM算法

  deadline越早，优先级越高。

  <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201108200816.png" style="width:80%;" />  

+ 最优性比较

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20201108204348.png" alt="image-20201108204348326" style="width:80%;" />

+ RM schedulability tests
  + Necessary: $U = \sum_{i=1}^{n}U_i \le 1$ .
  + **Liu and Layland’s test for RM**：时间利用率满足 $U \le n\cdot(2^{\frac{1}{n}}-1)$ .
  + **Hyperbolic bound test**: $\prod_{i=1}^{n}(U_i+1) \le 2$ .

