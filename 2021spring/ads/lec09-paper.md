## MapReduce

- Date: 2021/04/26
- Paper: [MapReduce: Simplified Data Processing on Large Clusters](https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf)

本周的论文是 Google 的「三驾马车」之一：MapReduce 。

MapReduce 整个过程可以用一张图来描述。

大白话：

- 把大文件分为若干个体积相等的小文件
- 由 master 节点分配不同的 split 到 Mapper Woker （一个 Mapper Worker 可能处理多个 split ）
- Mapper Worker 把中间结果写入本地文件，并告知 master 中间结果的位置
- master 把中间结果分配给 Reducer Worker，Reducer 合并这些中间结果并写入到输出文件

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210425152710.png" style="width:90%;" />





## References

- [1] [MapReduce: Simplified Data Processing on Large Clusters](https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf)



## Submission

**Synopsis**

This paper describes MapReduce, a programming model, which help users to process large-scale data through parallel computing. The MapReduce model mainly include two user interfaces:

- **Mapper** - which takes an input pair and pro- duces a set of intermediate key/value pairs, and 
- **Reducer** - which merge the result from Mapper(s) by key .

And this paper describes some methods to refine MapReduce in Chapter 4, such as using a partitioning function or a combiner function, and skipping bad records. At last, the paper measures performance of MapReduce through distributed applications "grep" and "sort".



**Question**

> In MapReduce each Mapper saves intermediate key/value pairs in R partitions on its local disk. Contrast the pros and cons of this approach to the alternative of having Mappers directly send intermediate results to R reducers that shuffle and save intermediate results on reducers' local disk before feeding them to the user-defined reduce function.

The advantage of the latter method is that the completed map worker will not re-executed when it fails, since the results are send to reducers (but the former method will re-execute in such case). 

The advantage of the former method is that there is only one copy of intermediate results (and there are two in the latter). Since the data size is usually very large, it will take up less memory and disk space in the clusters than the latter.



