## Tiled-MapReduce

- Date: 2021/05/23
- Paper: [Tiled-MapReduce: Optimizing Resource Usages of Data-parallel Applications on Multicore with Tiling](https://ipads.se.sjtu.edu.cn/courses/ads/paper/p523-chen.pdf)



一些无关重要的点:

- Phoenix uses the pthread library to assign tasks among CPU cores and relies on shared memory to handle inter-task communications. (Phoenix 是一个把 MapReduce 部署在多核单机上的系统)
  - fast inter-task communications in shared memory (因为节点之间只需要 IPC 而不需要 Network communication )



## Submission

**Synopsis**

This paper describes Tiled-MapReduce (TMR), a progamming model extended from MapReduce. TMR works on a single multi-core machine, and it iteratively process small-chunks of data in turn rather than a large chunk (which shows better performance) .

And the paper implemented a prototype of TMR based on Phoenix, called "Ostrich". Over Intel machine with 16 cores, the experiements on Ostrich shows that it is 1.2X - 3.3X faster than MapReduce.

**Question**

> Why does Tiled-MapReduce iteratively process **small trunks** of data instead of large chunks as traditional MapReduce?

**My Answer**

- TMR is based on a multi-core machine, more precisely, is based on IPC and shared-memory on single machine with multi-cores. Thus, the input and intermediate result will persist along the entire life cycle of "map" or "reduce" phases.

- A large-scale data can cause resource pressures on the runtime, OS and the caches, and this will decrease the performance.
- The small trunks can bring better cache/memory locality and less resource competition.