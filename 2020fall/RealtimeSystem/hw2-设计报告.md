## 一、引言

在开源实时系统内核 POK Kernel上进行如下设计：

+ 修改内核代码，使得内核中的多分区调度和线程调度支持更多的调度算法；

+ 设计应用场景，并编码实现；

+ 修改内核代码，使得POK的多线程调度支持动态增加线程；

+ 实现 MLFQ 调度算法；

+ 在 POK Kernel 中实现 MLFQ 算法。

实验代码：https://github.com/sinkinben/pok.git

## 二、线程调度算法的设计与实现

### 2.1 内核数据结构的修改

为了在 POK Kernel 中实现更多的线程调度算法，需要修改内核中相关的数据结构，以支持下面 3 种调度算法的算法特性。

首先是内核线程 `pok_thread_t` ，新增 2 个成员变量，即权重 `weight` 和到达时间 `arrive_time` , 因此在 `kernel/include/core/thread.h` 中修改该数据结构：

```c
typedef struct
{
	// ...
	/* stack pointer
	 * FIXME: this is platform-dependent code, we have to handle that ! */
	int weight;
	uint64_t arrive_time;
} pok_thread_t;
```

与之对应，同样需要修改线程的属性结构 `pok_thread_attr_t`，新增成员变量权重 `weight` 和到达时间 `arrive_time` ：

```c
typedef struct
{
    // ...
    int weight;
	uint64_t arrive_time;
} pok_thread_attr_t;
```

最后，为了支持 Weight Round Robin 算法的实现，需要修改内核分区的数据结构 `pok_partition_t` ，新增成员变量 `current_weight` ，该成员变量表示该分区当前执行线程的权重：

```c
typedef struct
{
    // ...
    // added for support WRR algorithm, should be 0 at initialization phase
    int current_weight;  
} pok_partition_t;
```

### 2.2 线程调度的工作机制

内核对分区和线程的调度是从函数 `pok_sched()` 开始的，该函数位于 `kernel/core/sched.c` 下。

（简述该函数的流程）



在 POK Kernel 中添加一种新的线程调度算法的流程如下：

+ 根据需要修改 `pok_thread_t` 的数据结构，并在 `xxx` 中完成初始化
+ 在 `.h` 中新增枚举变量
+ 在 `sched.c` 中实现该调度算法
+ 在 `switch case` 中添加分支

在目标程序（即POK Kernel的用户程序）中，可以通过在 `kernel/deployment.h` 中声明如下的宏定义，指定某个分区使用特定的线程调度算法：

```c
#define POK_XXX
```



### 2.3 EDF



### 2.4 Weight Round Robin



### 2.5 Fixed Priority



## 三、应用场景



### 3.1 生产者-消费者问题



### 3.2 分区1：单生产者-消费者





### 3.3 分区2：多生产者-消费者





## 四、MLFQ算法





## 五、总结

