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



### Quantitative Analysis (Execution Time)

+ 最坏执行时间 WCET: Worst Case Execution Time
+ 最好执行时间 BCET: Bes Case Execution Time
+ 利用程序的 CFG (Control Flow Graph) 图进行分析 WCET
+ 影响执行时间的因素：循环上界（算法复杂度）、路径空间（Exponential Path Space）、无效路径（Infeasilble Paths）、处理器行为（Processor Behavior: Cache Effect）



## Lecture-3 Online Scheduling of Aperiodic Tasks
+ 调度策略
  + 非抢占：先来先服务（FIFO）
  + 抢占：最早截止时间优先 EDF、短作业优先 SJF、固定优先级调度 FP

+ 要求总时延最低，怎么实现调度？



## Lecture-4

+ 如果一个 Task Set 是可调度的，那么使用 EDF 这个 Set 也是可调度的。证明之。