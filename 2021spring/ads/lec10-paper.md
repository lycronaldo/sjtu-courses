## Distributed GraphLab

- Date: 2021/05/08
- Paper: [Distributed GraphLab: a Framework for Machine Learning and Data Mining in the Cloud](https://ipads.se.sjtu.edu.cn/courses/ads/paper/ads-graphlab.pdf)





## Submission

**Synopsis**

This paper extend GraphLab to distributed version and also preserves strict consistency requirements in it. And in the distributed GraphLab:

- it optimizes the efficiency of network by data versioning and pipelined locking,
- fault tolerance is added (implemented by Chandy-Lamport snapshot algorithm).

**Question**

> How does distributed GraphLab provide consistency in parallel computing, and which consistency is supported by distributed GraphLab?

**My Answer**

The *sequential consistency model* is supported by distributed GraphLab. And the consistency model is designed into 3 levels:

- Full consistency model: the update function has complete read-write access to its entire scope.
- Edge consistency model: the update function has read-write access to its vertex and adjacent edges, but only read access to adjacent vertices.
- Vertex consistency model: it only provides write access to the central vertex data.