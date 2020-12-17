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

然后，为了支持 Weight Round Robin 算法的实现，需要修改内核分区的数据结构 `pok_partition_t` ，新增成员变量 `current_weight` ，该成员变量表示该分区当前执行线程的权重：

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
/*
 * Default scheduling algorithm is Round Robin.
 */
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

### 2.3 EDF



### 2.4 Weight Round Robin



### 2.5 Fixed Priority



## 三、应用场景

为了对比与测试不同调度算法对 POK Kernel 性能的影响，设计包含 4 个分区的用户程序：

+ 分区 1：单生产者-消费者模型
+ 分区 2：多生产者-消费者模型
+ 分区 3 和分区 4：为了测试多分区调度算法的有效性，均设置为单线程分区

本章节相关代码均位于 `pok/examples/mydemo` 文件夹下。

### 3.1 生产者-消费者问题

生产者消费者问题（Producer-consumer Problem），也称有限缓冲问题（Bounded-buffer problem），是一个多进程同步问题的经典案例。该问题描述了共享缓冲区的两个线程——即所谓的“生产者”和“消费者”，在实际运行时会发生的问题。生产者的主要作用是生成一定量的数据放到缓冲区中，然后重复此过程。与此同时，消费者也在缓冲区消耗这些数据。该问题的关键就是要保证生产者不会在缓冲区满时加入数据，消费者也不会在缓冲区为空时消耗数据 [[1]](#ref1)。

对于生产者和消费者，它们分别对应于分区中的一个线程。

对于缓冲区，在这里采用了一个环形队列来实现（实质是通过数组模拟），将在下面的 3.2 节阐述实现细节。

### 3.2 缓冲区

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

### 3.3 分区1：单生产者-消费者

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



### 3.4 分区2：多生产者-消费者

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



## 四、MLFQ算法





## 五、总结



## 参考文献

+ <span id="ref1">[1] [Producer–consumer problem](https://en.wikipedia.org/wiki/Producer–consumer_problem)</span>

