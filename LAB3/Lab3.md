# Lab 3-PB20000178李笑
## Q1: 最短区间长度

这个问题可以通过双指针算法解决，时间复杂度为 O(n) ，其中 n 为序列 array 的长度。

具体的步骤如下：

1. 首先，在输入序列数据的时候我们定义一个 sum 表示所有序列的和。如果 sum < K，表明所有序列相加都无法满足题目要求大于等于 K，所以我们可以直接输出 0，并结束程序；

2. 定义函数 `shortestInterval()` 用来寻找满足条件的最短区间长度；

3. 在函数中定义左右指针 left 和 right，初始值都为 0 ，以及当前的区间和 sum ，初始值为 array[0]；

4. 初始化最小长度 min_len 为 n+1 ，表示当前还没有找到符合条件的区间；

5. 不断移动右指针 right，更新区间和 `sum=sum+a[right]` ，直到 sum 大于等于 K；

6. 当 sum 大于等于 K 时，记录当前区间长度，即 `min_len=min(min_len, right-left+1)`；

7. 不断移动左指针 left，更新区间和 `sum=sum-a[left]`，直到 sum 小于 K；

8. 重复步骤 3-5，直到 right 到达序列 array 的末尾；

9. 返回 min_len。

```C
int shortestInterval(int *array, int n, int K) {
    int left = 0, right = 0;
    int sum = array[0], min_len = n + 1;

    while(right < n) {
        if(sum < K) {
            right ++;
            if(right < n) {
                sum += array[right];
            }
        }
        else {
            min_len = min_len < right - left + 1 ? min_len : right - left + 1;
            sum -= array[left];
            left++;
        }
    }
    return min_len;
}
```

以上做法的正确性建立在输入序列没有负数的情况下。

当 a 中存在负数时，问题会变得更加复杂。因为区间和的大小不再一定随着右指针的移动而增加，也不一定随着左指针的移动而减小。因此，我们需要对代码进行响应修改，以确保正确性。下面是代码的具体实现：
```C
int shortestInterval(int *array, int n, int K) {
    int min_len = n + 1;

    for (int len = 1; len <= n; len++) {
        int left = 0, right = 0;
        int sum = 0;
        for(int j = 0; j < len; j++) {
            sum += array[j];            
        }
        right = len;
        while(right <= n) {
            // printf("sum:%d\n", sum);
            if(sum >= K) return len;
            sum += array[right++];
            sum -= array[left++];
        }
    }
    
    return min_len;
}
```
因为加入了对序列长度的枚举以保证在序列中含有负数的情况下算法依然正确，所以算法的时间复杂度为 $O(n^2)$。

## Q2: 最小覆盖集
对于每个加入覆盖集的结点有如下性质：
1. 它的子树深度不超过 r 的结点全部可以被覆盖
2. 离该结点距离不超过 r 的结点会被覆盖

算法思路：  
首先，判断树的深度与 r 的关系。若小于等于，则直接取结点 1 就可覆盖所有其他结点。否则，我们可以使用`DFS()`先找到深度最大的结点，然后通过`findRParent()`回溯到可以把这个结点覆盖的深度最小的点（即该结点的第 r 代祖先）。接着将找到的这个结点加入覆盖集，并使用`update()`函数标记被覆盖的结点。寻找未被覆盖的深度最深的结点，重复上述过程，即可得到线性时间 $O(n)$ 算法。
![](images/lab3_2.png)

主要函数实现如下：
```C++
int father[MAX], child[MAX], num[MAX], cover[MAX], depth[MAX], dfs[MAX];
// father[i] 表示第 i 个结点的父亲，初始化为 -1
// num[i] 表示第 i 个结点的孩子数量，初始化为 0
// child[i] 初始化为 -1，child[num[i-1]+1]~child[num[i]] 记录的是第 i 个结点的孩子
// cover[i] 表示第 i 个结点是否被覆盖，初始化为 0
// depth[i] 表示第 i 个结点的深度，初始化为 0 （结点 1 的深度定义为 0）
// dfs[i] 记录的是广度优先遍历的结点顺序，dfs[]中最后一个结点深度最深

// 查找最大结点深度，若小于等于 r，则可直接输出 1 并结束程序
int findMaxDepth() {
    int max = -1;
    for(int i = 1; i <= n; i++) {
        max = (depth[i] > max ? depth[i] : max);
    }
    return max;
}

// 按深度顺序将结点存放在 dfs 数组中
void DFS() {
    dfs[1] = 1;
    int start = 0, end = 1;
    while(start != end) {
        start ++;
        for(int i = num[dfs[start]-1]+1; i <= num[dfs[start]]; i++) {
            dfs[++end] = child[i];
        }
    }
}

// 回溯找到结点的 r 代祖先
int findRParent(int a, int r) {
    if(a < 0) 
        return -1;
    if(r == 0)
        return a;
    return findRParent(father[a], r-1);
}

// 更新结点覆盖状态
void coverChild(int a, int r) {
    if((a < 0) || (r < 0)) return;
    for(int i = num[a-1] + 1; i <= num[a]; i++) {
        cover[child[i]] = 1;
        coverChild(child[i], r-1);
    }
}
void update(int a, int r) {
    if((a < 0) || (r < 0)) return;
    cover[a] = 1;
    coverChild(a, r-1);
    update(father[a], r-1);
}

// main()
count = 0 ;
for(int i = n; i > 0; i--) {
    if(cover[dfs[i]] == 0) {
        int a = findRParent(dfs[i], r);
        if(a < 0) {
            count++;
            printf("%d\n", count);
            return 0;
        }
        count++;
        update(a, r);
    }
}
```

在作业 hw3 中我们已经证明了该算法的正确性，在此重新列出：
1. 结点存在：因为一定有结点去把深度最大的结点覆盖，所以这样的结点存在；
2. 贪心选择包含在某些最优解中：对于某一最小覆盖集，如果他选择深度最小的结点去覆盖最深结点，那么我们的选择就是最优覆盖集的选择；如果他不选择深度最小的结点去覆盖最深结点，那么我们把这个结点从覆盖集中去掉，用深度最小的结点去覆盖最深结点，这时可以发现新的结点覆盖的范围比原结点大，新的集合也是覆盖集且大小不变，所以也是最优解；
3. 原问题和子问题有相同结构，可以归纳：子问题覆盖了除原问题贪心选择去掉结点以外的所有结点，因此子问题加上原问题的贪心选择可以合并出原问题的解；
4. 子问题的最优解可以还原原问题的最优解：令原问题为 $P$，假设贪心选择为 $c$， $P^′$ 为子问题。由子问题最优解 $π^′$ 还原得到原问题的解 $π = π^′ ∪ c$。假设 $π$ 不是最优解，并且存在最优解 $π^∗$，那么由前知贪心选择也可以在与 $π^∗$ 相同规模的覆盖集中 (所以就不妨设为在 $π^∗$ 中)，所以存在子问题 $P^′$ 上有更优解 $π^∗$。而这与 $π^′$ 为子问题 $P^′$ 上最优解矛盾。故子问题最优解可以还原原问题最优解。


## Q3: 递增子序列数量
使用动态规划算法，数组 dp[i][j] 表示以第 i 个元素结尾，长度为 j 的递增子序列个数，递推关系式为 $dp[i][j] = \sum_{t=1}^{i-1}dp[t][j-1], 2\leq j\leq k$ 。

```C
for(int i = 1; i <= n; i++) {
    for(int j = 2; j <= k; j++) {
        for(int t = 1; t < i; t++) {
            if(a[i] > a[t]) 
                dp[i][j] = (dp[i][j] + dp[t][j-1]) % 1000 ;
        }
    }
}

int ans = 0;
for(int i = 1; i <= n; i++)
ans = (ans+dp[i][k])%1000;
printf("%d\n", ans);
```

这个算法是最基础的动态规划算法，时间复杂度为 $O(n^2k)$，顺利通过了前五个样例，但是在后五个样例上 `Time Limited Exceeded`，说明我们还需要优化算法的时间。

我们采用树状数组+离散化的方法来优化该算法的时间复杂度，同时因为每次递归只用到了 j 和 j-1 列的存储数据，所以我们可以采用循环数组来优化该算法的空间复杂度。

树状数组(Binary Index Tree, BIT)支持单点修改和区间查询，时间复杂度均为 $O(logn)$。

```C++
#define lowbit(x) ((x) & (-x))
// 单点修改
int tree[MAXN];
inline void update(int i, int x)
{
    for (int pos = i; pos < MAXN; pos += lowbit(pos))
        tree[pos] += x;
}
// 求前 n 项的和
inline int query(int n)
{
    int ans = 0;
    for (int pos = n; pos; pos -= lowbit(pos))
        ans += tree[pos];
    return ans;
}
// 区间查询
inline int query(int a, int b)
{
    return query(b) - query(a - 1);
}
```

区间查询和更新操作可以替代前述动态规划算法的
```C
for(int t = 1; t < i; t++) {
    if(a[i] > a[t]) 
        dp[i][j] = (dp[i][j] + dp[t][j-1]) % 1000 ;
}
```
能有效降低算法的时间复杂度。

离散化是当我们只关心数据的大小关系时，用排名代替原数据进行处理的一种预处理方法。离散化的本质是一种哈希，它在保持原序列大小关系的前提下把其映射成正整数。离散化操作可以降低算法的时间复杂度。
```C++
void dis() {
    for(int i = 1; i <= n; i++) 
        b[i] = a[i];
    sort(b+1, b+1+n);
    int len=unique(b+1, b+1+n)-(b+1);
    for(int i = 1; i <= n; i++) 
        a[i] = lower_bound(b+1, b+1+len, a[i])-b;
}
```

改进后的算法时间复杂度为 $O(nklogn)$，成功通过所有样例。