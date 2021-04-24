## MapReduce

- Date: 2021/04/26
- Paper: [MapReduce: Simplified Data Processing on Large Clusters](https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf)

本周的论文是 Google 的「三驾马车」之一：MapReduce 。





## References

- [1] [MapReduce: Simplified Data Processing on Large Clusters](https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf)



## Submission

**Synopsis**

This paper describes MapReduce, a programming mode, which help users without any experience in distributed system to process large-scale data through parallel computing. The MapReduce model mainly include two user interfaces:

- **Mapper** - which takes an input pair and pro- duces a set of intermediate key/value pairs, and 
- **Reducer** - which merge the result from Mapper(s) by key .





**Question**

> In MapReduce each Mapper saves intermediate key/value pairs in R partitions on its local disk. Contrast the pros and cons of this approach to the alternative of having Mappers directly send intermediate results to R reducers that shuffle and save intermediate results on reducers' local disk before feeding them to the user-defined reduce function.





