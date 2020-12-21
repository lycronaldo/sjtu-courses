## 目录
- [一、引言](#一引言)
- [二、线程调度算法的设计与实现](#二线程调度算法的设计与实现)
   - [2.1 数据结构的修改](#21-数据结构的修改)
   - [2.2 线程调度的工作机制](#22-线程调度的工作机制)
   - [2.3 调度算法的实现](#23-调度算法的实现)
      - [2.3.1 EDF](#231-edf)
      - [2.3.2 Weighted Round Robin](#232-weighted-round-robin)
      - [2.3.3 Fixed Priority](#233-fixed-priority)
- [三、分区 WRR 调度的设计与实现](#三分区-wrr-调度的设计与实现)
   - [3.1 准备工作](#31-准备工作)
   - [3.1 数据结构的修改](#31-数据结构的修改)
   - [3.2 实现 WRR](#32-实现-wrr)
- [四、应用场景](#四应用场景)
   - [4.1 生产者-消费者问题](#41-生产者-消费者问题)
   - [4.2 缓冲区](#42-缓冲区)
   - [4.3 分区1：单生产者-消费者](#43-分区1单生产者-消费者)
   - [4.4 分区2：多生产者-消费者](#44-分区2多生产者-消费者)
- [五、MLFQ算法](#五mlfq算法)
   - [5.1 MLFQ 算法简述](#51-mlfq-算法简述)
      - [5.1.1 Background](#511-background)
      - [5.1.2 Design](#512-design)
      - [5.1.3 Optimization](#513-optimization)
      - [5.1.4 Variants](#514-variants)
   - [5.2 MLFQ算法实现](#52-mlfq算法实现)
      - [5.2.1 任务调度决策](#521-任务调度决策)
      - [5.2.2 Priority boost](#522-priority-boost)
      - [5.2.3 每个时间片的状态设置和检查](#523-每个时间片的状态设置和检查)
      - [5.2.4 对Gaming Attack的防止措施](#524-对gaming-attack的防止措施)
   - [5.3 代码实现](#53-代码实现)
- [六、总结](#六总结)
- [参考文献](#参考文献)

## 一、引言

在开源实时系统内核 POK Kernel上进行如下设计：

+ 修改内核代码，使得内核中的多分区调度和线程调度支持更多的调度算法；

+ 设计应用场景，并编码实现；

+ 修改内核代码，使得POK的多线程调度支持动态增加线程；

+ 实现 MLFQ 调度算法；

+ 在 POK Kernel 中实现 MLFQ 算法。

实验代码：https://github.com/sinkinben/pok.git

## 二、线程调度算法的设计与实现

### 2.1 数据结构的修改

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

然后，为了支持 Weighted Round Robin 算法的实现，需要修改内核分区的数据结构 `pok_partition_t` ，新增成员变量 `current_weight` ，该成员变量表示该分区当前执行线程的权重：

```c
typedef struct
{
    // ...
    // added for support WRR algorithm, should be 0 at initialization phase
    int current_weight;  
} pok_partition_t;
```

同时，上述 3 个数据结构新增的成员变量，需要完成初始化操作，以下分别是 `pok_thread_t` 和 `pok_partition_t` 完成初始化的地方：

+ `pok_thread_t` : 在 `thread.c` 中的  `pok_thread_init()` 函数；
+ `pok_partition_t` : 在 `partition.c` 中的 `pok_partition_init()` 函数。

然而，对于用户程序来说，内核中的数据结构是不可见的，因此需要提供 API，允许用户程序通过这些 API 访问内核提供的服务。在 POK Kernel 当中，这些 API 是通过 `libpok` 实现的。为了允许用户程序自定义线程的 `arrive_time` 和 `weight` ，需要在 `libpok/include/core/thread.h` 中修改 `pok_thread_attr_t` 的数据结构：

```c
typedef struct
{
    // ...
    int weight;
    uint64_t arrive_time;
} pok_thread_attr_t;
```

修改完成后，我们可以在用户程序中通过如下方式定义线程新增的 `arrive_time` 和 `weight` 属性：

```c
uint32_t tid;
pok_ret_t ret;
pok_thread_attr_t attr;
attr.priority = 42;
attr.period = 20;
attr.time_capacity = 5;
attr.deadline = 100;
attr.entry = t1;
ret = pok_thread_create(&tid, &attr);
```

### 2.2 线程调度的工作机制

内核对分区和线程的调度是从函数 `pok_sched()` 开始的，该函数位于 `kernel/core/sched.c` 下，该函数的主要作用是先执行 `pok_elect_partition` 完成分区的调度，然在选中的分区中，执行 `pok_elect_thread` 完成线程的调度。

分区通过 `pok_partition_t` 来表示，该数据结构有一个成员变量 `sched_func` ，表示当前分区的线程调度策略。这个 `sched_func` 成员变量是在 `pok_partition_setup_scheduler()` 函数（位于 `partition.c` ）中的一个 `switch-case` 完成初始化：

```c
switch (((pok_sched_t[])POK_CONFIG_PARTITIONS_SCHEDULER)[pid])
{
case POK_SCHED_RMS:
    pok_partitions[pid].sched_func = &pok_sched_part_rms;
    break;
case POK_SCHED_RR:
    pok_partitions[pid].sched_func = &pok_sched_part_rr;
    break;
case POK_SCHED_WRR:
    pok_partitions[pid].sched_func = &pok_sched_part_wrr;
    break;
case POK_SCHED_EDF:
    pok_partitions[pid].sched_func = &pok_sched_part_edf;
    break;
case POK_SCHED_PRIORITY:
    pok_partitions[pid].sched_func = &pok_sched_part_priority;
    break;
case POK_SCHED_MLFQ:
    pok_partitions[pid].sched_func = &pok_sched_part_mlfq;
    break;
default:
    pok_partitions[pid].sched_func = &pok_sched_part_rr;
    break;
}
```

那么在用户程序当中，应当如何指定分区使用特定的线程调度策略呢？这需要在 `examples/demo/kernel/deployment.h` 中，声明如下的宏定义：

```c
#define POK_CONFIG_PARTITIONS_SCHEDULER {POK_SCHED_EDF, POK_SCHED_PRIORITY, POK_SCHED_WRR}
```

在预编译时期，编译器会把内核代码中的 `POK_CONFIG_PARTITIONS_SCHEDULER` 替换为数组 `{POK_SCHED_EDF, POK_SCHED_PRIORITY, POK_SCHED_WRR}` ，从而达到指定分区使用自定义的线程调度策略。

在 POK Kernel 中添加一种新的线程调度算法的流程如下：

+ 根据需要修改 `pok_thread_t` 的数据结构，并在 `pok_thread_init()` 中完成初始化；
+ 在 `schedvalues.h` 中新增标记调度算法的枚举变量；
+ 在 `sched.{h, c}` 中声明并实现该调度算法；
+ 在 `pok_partition_setup_scheduler()` 函数中的 `switch-case` 中添加分支。

在目标程序（即POK Kernel的用户程序）中，可以通过在 `kernel/deployment.h` 中声明如下的宏定义，指定某个分区使用特定的线程调度算法：

```c
#define POK_CONFIG_PARTITIONS_SCHEDULER {POK_SCHED_XXX, ...}
```

### 2.3 调度算法的实现

在原本的开源 Pok Kernel 代码中，仅实现了静态 Round Robin 调度算法的实现，为了使得 Pok Kernel 能够对不同的实时任务能够尽可能的在 deadline 前完成执行，我们增加了三种调度算法的实现。这样，用户可以针对要处理的任务的特征来任意选择线程间和分区间合适的调度算法来调度任务。

#### 2.3.1 EDF

所有的线程调度算法均在 `sched.c` 中实现。

在 EDF 算法中，在每一个新的就绪状态，调度器都是从那些已就绪但还没有完全处理完毕的任务中选择最早截止时间的任务，并将执行该任务所需的资源分配给它。在有新任务到来时，调度器必须立即计算 EDF，排出新的定序，即正在运行的任务被剥夺，并且按照新任务的截止时间决定是否调度该新任务。如果新任务的最后期限早于被中断的当前任务，就立即处理新任务。按照 EDF 算法，被中断任务的处理将在稍后继续进行。 

EDF 核心代码如下：

```c
uint32_t pok_sched_part_edf(const uint32_t index_low, const uint32_t index_high, 
                            const uint32_t prev_thread, const uint32_t current_thread)
{
   uint32_t res;
   uint32_t from;
   uint64_t deadline = (~0);
   if (current_thread == IDLE_THREAD) res = prev_thread;
   else res = current_thread;
   from = res;
   uint8_t i = 0;
   for (i = index_low; i <= index_high; i++)
   {
      if (pok_threads[i].state == POK_STATE_RUNNABLE &&
          pok_threads[i].deadline < deadline &&
          pok_threads[i].remaining_time_capacity > 0)
      {
         res = i;
         deadline = pok_threads[i].deadline;
      }
   }
   if ((res == from) && (pok_threads[res].state != POK_STATE_RUNNABLE))
      res = IDLE_THREAD;
   return res;
}

```

#### 2.3.2 Weighted Round Robin

实现细节可参考<span>[[2]](#ref2)</span> .

WRR 算法的伪代码描述如下：

```c
while (true) {
    i = (i + 1) mod n;
    if (i == 0) {
        cw = cw - gcd(S); 
        if (cw <= 0) {
            cw = max(S);
            if (cw == 0)
            return NULL;
        }
    } 
    if (W(Si) >= cw) 
        return Si;
}
```

其中：

+ 任务集合为 $S = {S_0, S_1, \dots, S_{n-1}}$，$W(S_i)$ 表示 $Si$ 的权值；
+ 变量 $i$ 表示上一次选择的任务；
+ 变量 $cw$ 表示当前调度的权值；
+ $max(S)$ 表示集合 $S$ 中所有任务的最大权值；
+ $gcd(S)$ 表示集合 $S$ 中所有任务权值的最大公约数；
+ 变量 $i$ 初始化为 -1，$cw$ 初始化为 0 。

WRR 代码实现如下：

```c
uint32_t pok_sched_part_wrr(const uint32_t index_low, const uint32_t index_high, 
                            const uint32_t prev_thread, const uint32_t current_thread)
{
   uint32_t res;
   uint32_t from;
   if (current_thread == IDLE_THREAD) res = prev_thread;
   else res = current_thread;
   from = res;
   int weight_gcd = get_threads_weight_gcd(index_low, index_high);
   int weight_max = get_threads_weight_max(index_low, index_high);
   uint32_t i = index_low - 1;
   int cw = 0;
   if (pok_partitions[pok_current_partition].prev_thread != IDLE_THREAD)
      i = pok_partitions[pok_current_partition].prev_thread;
   if (pok_partitions[pok_current_partition].current_weight != 0)
      cw = pok_partitions[pok_current_partition].current_weight;
   while (1)
   {
      i = i + 1;
      if (i == index_high + 1) i = index_low;
      if (i == index_low)
      {
         // 在每个分区的数据结构 pok_partition_t 中记录当前的 cw
         pok_partitions[pok_current_partition].current_weight = cw = cw - weight_gcd;
         if (cw <= 0)
         {
            pok_partitions[pok_current_partition].current_weight = cw = weight_max;
            if (cw == 0)
            {
               res = IDLE_THREAD;
               break;
            }
         }
      }
      if (pok_threads[i].weight >= cw)
      {
         res = i;
         break;
      }
   }
   if ((res == from) && (pok_threads[res].state != POK_STATE_RUNNABLE))
      res = IDLE_THREAD;
   return res;
}

```

#### 2.3.3 Fixed Priority

固定优先级 (Fixed Priority) 调度算法的特点是把处理器分配给优先权最高的进程，但在执行期间，会不停的遍历线程或分区，只要出现另一个优先权更高的进程，则进程调度程序就立即停止当前进程的执行，并将处理器分配给新到的优先权最高的进程。

FP 调度算法如下：

```c
uint32_t pok_sched_part_priority(const uint32_t index_low, const uint32_t index_high, const uint32_t prev_thread, const uint32_t current_thread)
{
   const uint8_t max_priority = 0xff;

   uint32_t res;
   uint32_t from;
   uint8_t priority = max_priority;
   if (current_thread == IDLE_THREAD) res = prev_thread;
   else res = current_thread;
   from = res;
   uint8_t i = 0;
   for (i = index_low; i <= index_high; i++)
   {
      if (pok_threads[i].state == POK_STATE_RUNNABLE &&
          pok_threads[i].priority < priority &&
          pok_threads[i].remaining_time_capacity > 0)
      {
         res = i;
         priority = pok_threads[i].priority;
      }
   }
   if ((res == from) && (pok_threads[res].state != POK_STATE_RUNNABLE))
      res = IDLE_THREAD;
   return res;
}

```



## 三、分区 WRR 调度的设计与实现

本章节主要内容是在 POK Kernel中实现多分区的 WRR (Weighted Round Robin) 调度。

### 3.1 准备工作

为了不影响内核其他部分的正常工作，我们把与分区 WRR 调度的代码通过条件编译的方式隔离：

```c
#ifdef POK_NEEDS_PARTITIONS_SCHEDULER
// do something here
#endif
```

如果用户程序需要使用多分区 WRR 调度，那么需要在 `deployment.h` 中配置：

```c
#define POK_NEEDS_PARTITIONS_SCHEDULER 1
#define POK_CONFIG_PARTITIONS_TYPE POK_SCHED_WRR
#define POK_CONFIG_PARTITIONS_WEIGHT {2,3,4}
```

三者缺一不可，`POK_CONFIG_PARTITIONS_TYPE` 表示使用何种多分区调度策略，`POK_CONFIG_PARTITIONS_WEIGHT` 表示使用 WRR 调度分区时，用于指定各个分区的权重。

为了实现的方便，在这里做了一点小小的改进，WRR 算法只会执行一遍，当执行完成后，会根据原来定义的 `POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION` 序列依次执行（即 RR 算法）。

例如，在上述权重分别为 `{2,3,4}` 的情况下，WRR 产生的序列为 `P3, P2, P3, P1, P2, P3, P1, P2, P3`，随后内核会切换为调度策略，依次根据序列 ``POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION`` 执行。

### 3.1 数据结构的修改

修改 `partition.h` 中 `pok_partition_t` 添加属性 `weight`，表示：

```c
typedef struct
{ 
  // ...
  // cw for supporting WRR thread schduler, should be 0 at initialization phase
  int current_weight;    
  // weight for WRR partition scheduler
  uint8_t weight;
} pok_partition_t;
```

在 `pok_partition_init()` 中初始化权重 `weight`:

```c
#ifdef POK_NEEDS_PARTITIONS_SCHEDULER
      pok_partitions[i].weight = ((uint8_t[])POK_CONFIG_PARTITIONS_WEIGHT)[i];
#endif
```

### 3.2 实现 WRR

分区的调度是通过 `pok_elect_partition()` 函数完成的，因此需要在该函数中加入：

```c
#ifndef POK_NEEDS_PARTITIONS_SCHEDULER
    pok_sched_current_slot = (pok_sched_current_slot + 1) % POK_CONFIG_SCHEDULING_NBSLOTS;
    pok_sched_next_deadline = pok_sched_next_deadline + pok_sched_slots[pok_sched_current_slot];
    next_partition = pok_sched_slots_allocation[pok_sched_current_slot];
#else
    next_partition = wrr_next_partition();
    printf("executing partition = PR[%d]\n", next_partition + 1);
#endif
```

`wrr_next_partition()` 即是在 `sched.c` 中实现的 WRR 分区调度算法（与线程的 WRR 调度类似，不在此赘述）。 

## 四、应用场景

为了对比与测试不同调度算法对 POK Kernel 性能的影响，设计包含 3 个分区的用户程序：

+ 分区 1：单生产者-消费者模型
+ 分区 2：多生产者-消费者模型
+ 分区 3：为了测试多分区调度算法的有效性，设置为单线程分区

本章节相关代码均位于 `pok/examples/mydemo` 文件夹下。

### 4.1 生产者-消费者问题

生产者消费者问题（Producer-consumer Problem），也称有限缓冲问题（Bounded-buffer problem），是一个多进程同步问题的经典案例。该问题描述了共享缓冲区的两个线程——即所谓的“生产者”和“消费者”，在实际运行时会发生的问题。生产者的主要作用是生成一定量的数据放到缓冲区中，然后重复此过程。与此同时，消费者也在缓冲区消耗这些数据。该问题的关键就是要保证生产者不会在缓冲区满时加入数据，消费者也不会在缓冲区为空时消耗数据 [[1]](#ref1)。

对于生产者和消费者，它们分别对应于分区中的一个线程。

对于缓冲区，在这里采用了一个环形队列来实现（实质是通过数组模拟），将在下面的 4.2 节阐述实现细节。

### 4.2 缓冲区

缓冲区是一个环形队列，其数据结构如下：

```c
#define CAPACITY 4 // buffer 的容量
typedef struct
{
    char items[CAPACITY];
    int in, out;
} buffer_t;
```

其中，`in` 总是指向进队位置，`out` 总是指向出队位置。

同时实现如下 API 操作环形队列：

```c
// init buffer_t
void buffer_init(buffer_t *b)
{
    b->in = b->out = 0;
}
// Judge whether the buffer is full
int buffer_is_full(buffer_t *b)
{
    return ((b->in + 1) % CAPACITY) == (b->out);
}
// Judge whether the buffer is empty
int buffer_is_empty(buffer_t *b)
{
    return b->in == b->out;
}
// Put a item into buf
void buffer_put_item(buffer_t *buf, char item)
{
    buf->items[buf->in] = item;
    buf->in = (buf->in + 1) % CAPACITY;
}
// Get a item from buf
char buffer_get_item(buffer_t *buf)
{
    char item = buf->items[buf->out];
    buf->out = (buf->out + 1) % CAPACITY;
    return item;
}
```

### 4.3 分区1：单生产者-消费者

分区 1 是一个单一的生产者-消费者模型，其具体场景如下：

* 分区中有 3 个线程：生产者 `producer` 、计算者 `calculator` 、消费者 `consumer`
* 系统中有 2 个容量为 4 的缓冲区：`buffer1, buffer2`
* 生产者生产 `a-z` 字符，放入到 `buffer1`; 计算者从 `buffer1` 取出字符，将小写字符转换为大写字符，放入到`buffer2`
* 消费者从 `buffer2` 取出字符，将其打印到屏幕上

为了避免死锁和无效竞争等线程并发问题， 3 个线程之间需要通过信号量 (semaphore) 和互斥量 (mutex) 设计一种同步机制。而互斥量实质上是信号值为 1 的信号量，为了代码的简洁性，我们统一通过信号量来完成线程同步。

在 `examples/mydemo/main.c` 定义如下 6 个信号量：

```c
uint8_t empty1, full1, mutex1;
uint8_t empty2, full2, mutex2;
```

其作用分别如下：

+ `mutex1` 和 `mutex2` 的作用相当于互斥量，是为了保证只有一个线程访问 `buf1` 或者 `buf2` 。
+ `empty1, empty2` 的信号量值将被初始化为 `buff` 的容量，表示可用的临界资源的数量（指 `buff` 中空闲位置的个数）。
+ `full1, full2` 的信号量的值将被初始化为 0，表示可用的临界资源的数量（指 `buff` 中可取走的 `item` 的数量）。

最后来看一下生产者、计算者与消费者 3 个线程的具体实现。

在 `pr1/activity.c` 中，声明 2 个全局变量：

```c
static const int N = 30;
static const int limit = 26;
```

N 表示生产者、计算者、消费者均要完成 N 次操作；limit 表示生产者产生的字符范围是 `a-z` 共 26 个。

+ 生产者 `producer_job`

```c
void *producer_job()
{
   int i;
   char item = 'a';
   for (i = 0; i < N; i++)
   {
      pok_sem_wait(empty1, 0);
      pok_sem_wait(mutex1, 0);
      item = 'a' + (i % limit);
      buffer_put_item(&buf1, item);
      printf("[Producer] put item [%c] into buf1. \n", item);
      pok_sem_signal(mutex1);
      pok_sem_signal(full1);
   }
   return NULL;
}
```

+ 计算者 `calculator_job`

```c
void *calculator_job()
{
   int i;
   char item;
   for (i = 0; i < N;i++)
   {
      pok_sem_wait(full1, 0);
      pok_sem_wait(mutex1, 0);
      item = buffer_get_item(&buf1);
      pok_sem_signal(mutex1);
      pok_sem_signal(empty1);

      item = item - 'a' + 'A';

      pok_sem_wait(empty2, 0);
      pok_sem_wait(mutex2, 0);
      buffer_put_item(&buf2, item);
      pok_sem_signal(mutex2);
      pok_sem_signal(full2);
   }
   return NULL;
}
```

+ 消费者 `consumer_job`

```c
void *consumer_job()
{
   int i;
   char item;
   for (i = 0; i < N; i++)
   {
      pok_sem_wait(full2, 0);
      pok_sem_wait(mutex2, 0);
      item = buffer_get_item(&buf2);
      printf("\t[Consumer] get item [%c] from buf2. \n", item);
      pok_sem_signal(mutex2);
      pok_sem_signal(empty2);
   }
   return NULL;
}
```



### 4.4 分区2：多生产者-消费者

分区 2 是多生产者-消费者模型，共有 4 个线程，其具体场景如下：

+ 桌子上有一果盘，容量为 N 
+ 爸爸只会在盘子里放苹果，儿子只吃盘子中的苹果；
+ 妈妈只会在盘子里放橘子，女儿只吃盘子中的橘子；
+ 只有果盘有空余位置时，爸爸或妈妈就可向盘子中放入一个水果；
+ 仅当盘子中有自己需要的水果时，儿子或女儿可以从盘子中取出。

这是 PV 操作中的一个经典问题，又称“苹果-橘子问题”。

与上面类似，果盘被定义为一个环形队列：

```c
typedef struct
{
    uint8_t fruits[CAPACITY];
    uint32_t in, out;
} plate_t;
```

操作环形队列的 API 与 3.2 节所述相同，不在此赘述。

苹果和橘子定义为枚举变量，分别用 0 和 1 表示：

```c
typedef enum
{
    APPLE = 0,
    ORANGE
} fruit_t;
```

所使用的信号量：

```c
pok_sem_id_t apple, orange, empty, mutex;
```

其中：

+ `apple, orange` 分别表示果盘中苹果，橘子的数量；
+ `empty` 表示果盘中空闲位置的数量；
+ `mutex` 用于确保只有 1 个线程能访问缓冲区。

表示 `father` 和 `son` 的线程的具体实现如下（其余 2 个与之类似）：

```c
void *father_job()
{
   pok_time_get(&start);
   int i;
   for (i = 0; i < N; i++)
   {
      pok_sem_wait(empty, 0);
      pok_sem_wait(mutex, 0);
      printf("[Father] apple++ \n");
      plate_put_fruit(&plate, APPLE);
      pok_sem_signal(mutex);
      pok_sem_signal(apple);
   }
   return NULL;
}
void *son_job()
{
   int i;
   for (i = 0; i < N; i++)
   {
      pok_sem_wait(apple, 0);
      pok_sem_wait(mutex, 0);
      printf("\t[Son] %s-- \n", strs[plate_get_fruit(&plate)]);
      pok_sem_signal(mutex);
      pok_sem_signal(empty);
   }
   return NULL;
}
```



## 五、MLFQ算法

### 5.1 MLFQ 算法简述

#### 5.1.1 Background

存在 2 个因素之间的 tradeoff :

- turnaround time: SJF
- response time: Round Robin

操作系统无法得知一个进程的运行时间，要求在运行时得知任务的特征，并据此做出更好的调度决策。

MLFQ 能够根据历史数据，使用某种启发式算法，来做预测。

#### 5.1.2 Design

多个队列，每个队列有一个优先级。

不同队列之间使用优先级调度，同一队列中使用Round Robin调度。

通过观察到的任务行为来决定一个任务的优先级：

- 与外设交互比较多的任务（short-running interactive job），由于阻塞而让出CPU的时间较多，会获得较高的优先级
- 密集使用CPU时间的任务（long-running CPU-intensive job）的优先级会被降低

任务的优先级更改：

- 当有新的任务到达时，将其优先级设为最高
- 当一个任务连续使用了1个时间片后，其优先级会被下降，即被下移1个队列
- 当一个任务在使用完1个时间片之前就让出了CPU，那么该任务将留在同一队列中

例子分析：

- 短任务会留在较高优先级，长任务的优先级会逐渐下降，从而达到接近SJF的调度效果
- 交互型的任务会在使用完时间片前就让出CPU，使其一直处于最高的优先级，从而达到IO密集型的任务能被优先处理的调度效果

#### 5.1.3 Optimization

以下是基本的 MLFQ 存在的问题：

- 饿死问题：如果太多交互型的任务存在，就会导致低优先级的任务不被调度而饿死。
- Gaming：恶意用户可以在时间片用完前通过触发IO来让出CPU，从而获得大多数的CPU时间。
- 任务行为的改变：一个处于低优先级的CPU密集型任务可能会改变成交互密集型任务，使用基本的MLFQ设计会使得这些任务没有被优先调度。

解决方案：

- The Priority Boost：经过某个周期S，将所有任务放到最上面的队列中（S需要合适的设置）
- Better Accounting：一个任务间断的执行时间超过1个时间片，就降低其优先级

#### 5.1.4 Variants

1. 不同队列具有不同的时间片长度，使得高优先级的交互型任务尽可能多的被切换，低优先级的CPU密集型任务尽可能少的被切换。
2. Solaris MLFQ implementation：可以让系统管理员配置各个调度器参数。
3. 根据数学公式调整任务的优先级


### 5.2 MLFQ算法实现

#### 5.2.1 任务调度决策

每个时间片选择最高优先级队列中的队首任务进行执行。

#### 5.2.2 Priority boost

每隔一段时间进行一些priority boost，将所有队列中的任务移至最高优先级的队列当中，并重置所有任务的剩余时间配额。

#### 5.2.3 每个时间片的状态设置和检查

当前时间片执行任务的剩余执行时间和剩余时间配额需要自减1

- 检查是否有IO完成，若完成则将其加入到原队列当中
- 检查当前任务的剩余执行时间是否为0，若是，则说明该任务执行完成，将其从队列中删除，并记录任务完成
- 检查是否有任务需要执行IO（在本系统实现中，每个任务的IO频率固定），若需要，则将其从队列中删除，并记录IO结束时间
- 检查是否有任务的时间配额为0，若是，则将其移到更低优先级的队列当中，并将时间配额重置。若该任务已在最低优先级的队列当中，则只重置时间配额

#### 5.2.4 对Gaming Attack的防止措施

维护每个任务的剩余时间配额，在任务完成IO后不重置其时间配额，而是继续根据执行时间减少。

### 5.3 代码实现

参考文件 `mlfq/mlfq.py` ，可通过 `python3 mlfq.py` 直接运行并测试内置的测试用例 。


## 六、总结

本次实验基于 POK Kernel，主要完成以下工作：

+ 实现了单个分区内的三种线程调度算法：EDF，WRR，FP；
+ 实现多分区的 WRR 调度；
+ 设计并实现了 POK Kernel 的应用场景，一个包含 3 分区的多生产者-消费者模型；
+ 设计并实现了MLFQ算法。

## 参考文献

+ <span id="ref1">[1] [Producer–consumer problem](https://en.wikipedia.org/wiki/Producer–consumer_problem)</span>
+ <span id="ref2">[2] [Weighted Round-Robin Scheduling](http://kb.linuxvirtualserver.org/wiki/Weighted_Round-Robin_Scheduling)</span>

