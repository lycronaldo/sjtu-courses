## Unit 1 - Introduction

Date: 2021/03/23

The main content of this class is to introduce the syllabus of this course.

And, few simple algorithms knowledge is introduced.

## Master Theorem

The *Master Theorem* is used for solving the time complexity of the following recursive algorithm:
$$
T(n) = aT(\frac{n}{b}) + f(n)
$$
In the above formula,

- $n$ represents the scale of the original problem
- $a(a\ge1)$ represents the number of the sub-problems
- $n/b(b\ge2)$ represents the scale of each sub-problem (assume that each sub-problem has the same scale)
- $f(n)$ is the the total time of decomposing the original problem into sub-problems and merging the solutions of each sub-problem.

Expecially, when $f(n) = O(n^d)$ for $d \ge 0$, we can get the following conclusion:
$$
T(n) = 
\begin{cases}
O(n^d)             &if \quad d>\log_{b}{a} \\
O(n^d\log{n})      &if \quad d=\log_{b}{a} \\
O(n^{\log_{b}{a}}) &if \quad d<\log_{b}{a} \\
\end{cases}
$$


## Merge Sort

|                          Recursion                           |                          Iteration                           |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210323161742.png"/> | <img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210323161834.png"/> |

**Time Complexity Analysis of recursion**

It's easy for us to see that:
$$
T(n) = 2T(\frac{n}{2}) + O(n)
$$
According to *Master Theorem*, the constant $d = 1 = \log_{b}{a}$, thus $T(n) = O(n\log{n})$ .

**Code for Recursion Version**

```cpp
// merge [l, m] and [m+1, r]
void merge(vector<int> &v, int l, int m, int r)
{
    vector<int> buf(r - l + 1, 0);
    int i = l, j = m + 1, k = 0;
    while (i <= m && j <= r)
    {
        if (v[i] < v[j]) buf[k++] = v[i++];
        else buf[k++] = v[j++];
    }
    while (i <= m) buf[k++] = v[i++];
    while (j <= r) buf[k++] = v[j++];
    copy(buf.begin(), buf.end(), v.begin() + l);
}
void mergesort(vector<int> &v, int l, int r)
{
    int n = v.size();
    if (l < 0 || r < 0 || r >= n || l >= n) return;
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergesort(v, l, m);
        mergesort(v, m + 1, r);
        merge(v, l, m, r);
    }
}
```

**Code for Iteration Version**

```cpp
void mergesort2(vector<int> &v)
{
    int n = v.size();
    for (int k = 1; k < n; k *= 2)
    {
        for (int l = 0; l < n - 1; l += 2 * k)
        {
            int m = min(l + k - 1, n - 1);
            int r = min(l + 2 * k - 1, n - 1);
            merge(v, l, m, r);
        }
    }
}
```



## Lower Bound of Sorting

<img src="https://gitee.com/sinkinben/pic-go/raw/master/img/20210323172010.png" style="width:80%;" />



## Shortest Path in Graph

### BFS

How to fix BFS to adapte the

### Dijkstra

