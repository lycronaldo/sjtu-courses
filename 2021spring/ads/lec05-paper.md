## A Critique of ANSI SQL Isolation Levels

- Date: 2021/03/22
- Paper: [A Critique of ANSI SQL Isolation Levels](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/tr-95-51.pdf)

**Question**

> Snapshot isolation (SI) differs from serilizatiability due to one anomaly (异常) that is possible under SI but not under serilizatiability. Describe the anomality and also give a concrete application for which the anomaly is undesirable.

**My Answer**

This paper gives a more clear and comprehensive definition of anomalies and isolation levels compared with ANSI SQL-92 (see *table-4* in Chapter-5). And according to the new anomalies, two new isolation types, Cursor Stability and Snapshot Isolation, are defines in this paper.

The answer of the above question is in section 4.2.

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210321200136.png" style="width:60%;" />

## Abstract

> ANSI SQL-92 [MS, ANSI] defines Isolation Levels in terms of phenomena: Dirty Reads, Non-Repeatable Reads, and Phantoms. This paper shows that these phenomena and the ANSI SQL definitions fail to characterize several popular isolation levels, including the  standard locking implementations of the levels.
>
> Investigating the ambiguities of the phenomena leads to clearer definitions; in addition new phenomena that better  characterize isolation types are introduced. An important multiversion isolation type, Snapshot Isolation, is defined.

ANSI SQL-92 标准根据数据库的异常现象 (Phenomena) 定义隔离级别, 这些现象有:

- Dirty Reads (脏读)
- Non-Repeatable Reads (不可重复读)
- Phantoms (幻读)

但这 3 种定义不足以描述几种常见的隔离 (这篇 paper 是在 95 年发表的, 应该是数据库领域又出现了一些新的 Isolation Level), 例如 Standard locking  .

本文的目的是对 Isolation Level 给出一个更加清晰的定义, 并定义了一种 Multi-version Isolation Type: Snapshot Isolation.



## 1 Introduction

一般来说, 隔离级别越低, 约束条件越少, 事务的并发度就会越高. 

ANSI/ISO SQL-92 根据 Phenomena 定义了 4 种隔离级别:

- Read Uncommitted
- Read Committed
- Repeatable Read
- Serializable

Phenomena 在 ANSI/ISO SQL 中的具体描述是:

> Phenomena are action subsequences that may lead to anomalous (perhaps non-serializable) behavior. 

在本文中, 使用 *anomalies* 与 ANSI SQL 中的 Phenomena 加以区分, anomalies 是作者对 ANSI SQL Phenomena 的补充. 二者在技术细节上的区别将会在后文提出.

每个章节的内容: 

- Section 2 introduces **basic isolation level terminology**. It defines the ANSI SQL and locking isolation levels. 
- Section  3 examines some drawbacks of the ANSI isolation levels and proposes a new phenomenon.
- Section 4 introduces a multi-version concurrency control mechanism, called Snapshot Isolation, that avoids the ANSI SQL phenomena, but is not serializable. 
- Section 5 explores some new anomalies to differentiate the  isolation levels introduced in Sections 3 and 4.



## 2 Isolation Definitions

### 2.1 Serializability Concepts

相关定义:

- A ***transaction*** groups a set of actions that transform the database from one consistent state to another. 
  - 一个事务包含了一系列动作，将数据库从一个一致性状态转换到另一个。
- A ***history*** models the interleaved execution of a set of transactions as  a linear ordering of their actions.
  - 交叉执行的一系列事务的动作线性化。
- Serializability

### 2.2 ANSI SQL Isolation Levels





### 2.3 Locking



## 3 Analyzing ANSI SQL Isolation Levels



## 4 Other Isolation Types

### 4.1 Cursor Stability



### 4.2 Snapshot Isolation



### 4.3 Other Multi-Version Systems





## References

- https://blog.csdn.net/hnwyllmm/article/details/95947776
- https://zhuanlan.zhihu.com/p/38334464
- https://zhuanlan.zhihu.com/p/187597966