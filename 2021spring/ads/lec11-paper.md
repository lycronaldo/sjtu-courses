## PowerLyra

- Date: 2021/05/10
- Paper: [PowerLyra: Differentiated Graph Computation and Partitioning on Skewed Graphs](https://ipads.se.sjtu.edu.cn/courses/ads/paper/ads-powerlyra.pdf)

讲道理，最近几节课都没有好好读论文了，作业直接就是 Copy-Paste 😅 。



## Submission

**Synopsis**

This paper describes PowerLyra, a new graph computation engine which applies different computation and partitioning strategies for high-degree and low-degree vertices.

**Question**

> Please explain the claim in the paper "For high-degree vertices, the upper bound of increased mirrors due to assigning a new high-degree vertex along with in-edges is equal to the number of partitions (i.e. machines) rather than the degree of vertex".

**My Answer**

Since all the in-edges are assigned according to the hash value of the source vertex, the low-degree vertices do not create new mirrors. If the source vertex of a vertex is high-degree, then these two vertices will create 2 mirrors on one machine.

