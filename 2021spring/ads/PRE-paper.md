## GridGraph

- Date: 2021/05/17
- Paper: [GridGraph: Large-Scale Graph Processing on a Single Machine Using 2-Level Hierarchical Partitioning](https://www.usenix.org/conference/atc15/technical-session/presentation/zhu)



## 1 Introduction

- 现有的 Distributed graph processing system，例如 Pregel, GraphLab, PowerGraph, GraphX ，是通过大规模的机器集群提升性能的。
- 在 Graph Processing 的分布式场景下，load imbalance, synchronization overhead, fault tolerance overhead 仍是困难与挑战。
- GraphChi, X-Stream 等系统可以在单机上通过高效利用 disk 的方式，去处理大规模的图数据。
  - GraphChi (vertex-centric): its sharding process requires the edges in every shard to be sorted by sources, which is very time consuming.
  - X-Stream (edge-centric): scatter-gather model; lack of support on selective scheduling;
- GridGraph
  - groups edges into "grid" representation
  - vertices are partitioned into 1D chunks, edges are partitioned into 2D blocks
  - GridGraph combines the scatter and gather phases into one “streaming-apply” phase
  - offers selective scheduling, so that streaming on unnecessary edges can be reduced.

- Main contributions:
  - **A novel grid representation for graphs**, which can be generated from the original edge list using a fast range-based partitioning method.
  - **A 2-level hierarchical partitioning schema**, which is effective for not only out-of-core but also in- memory scenarios.
  - A fast streaming-apply graph processing model, which optimizes I/O amount.
  - **A flexible programming interface**, specify an optional user-defined filter function to skip computation on inactive vertices or edges.
- Paper organization
  - Section 2 describes the grid representation, which is at the core of GridGraph. 
  - Section 3 presents the computation model, and the 2-level hierarchical partitioning schema. 
  - Section 4 evaluates the system, and compares it with state-of-the-art systems. 
  - Section 5 discusses related works, and finally Section 6 concludes the paper.



## 2 Graph Representation

主要介绍图的表示方法。

### 2.1 The Grid Format

GraphChi：

- designes the shard data format, and stores the adjacency list in several shards so that each shard can be fit into memory.
- Vertices are divided into disjoint intervals. Each interval associates a shard, which stores all the edges with destination vertex in the interval.
- Inside each shard, edges are sorted by source vertex and combined into the compact adjacency format.

X-Stream:

- also divides the vertices into disjoint subsets.
- A streaming partition consists of a vertex set, an edge list and an update list, so that data of each vertex set can be fit into memory.

GridGraph:

- partitions the vertices into *P* equalized vertex chunks, each chunk contains vertices within a contiguous range.
- 



## References

- [1] https://zhuanlan.zhihu.com/p/38010945
- 