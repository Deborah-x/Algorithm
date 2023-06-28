# Lab 5-PB20000178李笑
## Q1: 最大流
### Edmonds-Karp 算法
该算法通过在 Ford-Fulkerson 算法寻找增广路径的操作中使用广度优先搜索来改善算法的效率。

BFS 实现：
```C++
bool BFS(int s, int t) {
    int p;
    queue<int> q;
    memset(pre, -1, sizeof(pre));
    memset(visit, false, sizeof(visit));
    pre[s] = s;
    visit[s] = true;
    q.push(s);
    while(!q.empty()) {
        p = q.front();
        q.pop();
        for(int i = 1; i <= n; i++) {
            if(r[p][i] > 0 && !visit[i]) {
                pre[i] = p;
                visit[i] = true;
                if(i == t)
                    return true;
                q.push(i);
            }
        }
    }
    return false;
}
```
在 BFS 寻找增广路径的同时，用 pre 数组记录每个节点的前一个节点，便于之后回溯。

Edmonds_Karp 实现：
```C++
int Edmonds_Karp(int s, int t) {
    int flow = 0;
    int d;
    while(BFS(s ,t)) {
        d = inf;
        // 找到增广路径中最小流量，如果d<r[pre[i]][i],把d的值赋给d，否则把r[pre[i]][i]的值赋给d
        for(int i = t; i != s; i = pre[i])
            d = d < r[pre[i]][i] ? d : r[pre[i]][i];
        for(int i = t; i != s; i = pre[i]) {
            r[pre[i]][i] -= d;  // 正
            r[i][pre[i]] += d;  // 反
        }
        flow += d;
    }

    return flow;
}
```
通过 BFS 查找一条增广路径，若存在，通过回溯找到增广路径中最小流量，然后更新残存网络边的权重。若不存在，说明已经达到最大流，返回最大流的值。

《算法导论》第26章已经证明了 Edmonds_Karp 算法的正确性，其时间复杂度为 $O(VE^2)$。

### Dinic 算法
算法步骤：

1. 在残留网络上 BFS 求出各个顶点的层次，构造分层图
2. 在分层图上进行 DFS ，沿着层次增 1 并且 flow < limit 的方向寻找增广路径，在回溯时实现更新剩余容量
3. 重复以上步骤，直到不存在增广路径为止

Dinic 的优势在于一次深度优先搜索可以找到多条增广路径，实现多次增流。

具体实现如下：
```C++
// u是当前节点
// limit表示从起点走到当前点的路径的容量上限
// 我们要在满足这个限制的基础上, 求出从当前点到汇点的最大流量
int Find(int u, int t, int limit) {
	// 如果u等于t，那么则找到了从源点s到汇点t的最大流，此时答案就是limit
	if(u == t)
		return limit;
	// flow表示u节点之后可以流出的流量
	int flow=0;
	// 遍历节点u的所有邻接点
	for(int i = 1; i <= n; i++) {
		if((d[i] == d[u] + 1) && (r[u][i] > 0)) {
			int temp = Find(i, t, min(r[u][i], limit-flow));
			r[u][i] -= temp;
			r[i][u] += temp;
			flow += temp;
		}
	}
	return flow;
}

int Dinic(int s, int t) {
	int maxflow = 0;
	int flow;
	while(BFS(s, t)) {
		while(flow = Find(s, t, inf))
			maxflow += flow;
	}
	return maxflow;
}
```
Dinic 算法的 BFS 与 Edmons-Karp 算法几乎一致，唯一的区别在于在遍历的同时需要维护一个节点的层次数组 d，而不需要保存父亲节点 pre。为节省空间，我们将 d 数组初始化为 -1，在访问时赋予深度，同时该数组可以取代 visit 数组用来标记节点是否已访问，若未访问， d[i] 的值应该为 -1。

在 Dinic 函数中，首先判断是否还存在增广路径，若存在，对其进行 DFS 更新残存网络权值。若不存在，说明已经找到最大流，直接返回 maxflow 即可。

Dinic 算法的时间复杂度为 $O(V^2E)$ ，因为边的数量 m 通常远远大于节点数量 n ，所以通常情况下 Dinic 算法比 Edmons-Karp 算法更快。在 OJ 的测试样例上，我们也可以看出，Dinic 算法所用的时间是 Edmons-Karp 算法 所用时间的一半，在图规模更大时此效果将会更明显。

## Q2: 重复块查找
这题也可以理解为是二维的字符串匹配问题。受 RK 算法启发，我们可以通过对两个数组计算哈希来完成线性时间复杂度的匹配。

具体实现如下：
```C
void matching() {
    for(int i = 1; i <= n2; i++) {
        for(int j = 1; j <= m2; j++) 
            B[i][j] = B[i][j] + p1*B[i][j-1] + p2*B[i-1][j] - p1*p2*B[i-1][j-1];
    }

    int t1 = pow(p1, m1), t2 = pow(p2,n1);
    for(int i = n1; i <= n2; i++) {
        for(int j = m1; j <= m2; j++) {
            hb = B[i][j] - t1*B[i][j-m1] - t2*B[i-n1][j] + t1*t2*B[i-n1][j-m1];
            if(hb == ha)
                printf("%d %d\n", i-n1, j-m1);
        }
    }
}
```

由于哈希计算是可递推的，计算矩阵 A 和矩阵 B 的哈希值时间复杂度分别为 $O(n1\times m1)$、$O(n2\times m2)$。而匹配时只需遍历查找，所以总时间复杂度为 $O(n2\times m2),(n2\geq n1,m2\geq m1)$。

## Q3: 取余
这题我们采用模幂算法来求解表达式 $a^{b^c}$ 的值。以下是对代码的详细分析和解释：

模幂算法函数：
```C
long long int power_mod(long long int base, long long int exponent, long long int modulus) {
    // printf("base:%d,exponent:%d,modulus:%d\n", base, exponent, modulus);

    if(base % modulus == 0)
        return 0;

    long long int result = 1;
    base = base % modulus;

    while(exponent > 0) {
        if(exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    // printf("result:%d\n", result);
    return result;
}
```
power_mod 函数实现了模幂算法，用于计算 $base^{exponent}\ mod\ modulus$ 的值。
首先，在 base 取模 modulus 后进行初始化操作，确保 base 在合适的范围内。
接下来，通过迭代计算的方式，使用位运算和模运算来计算幂值。
当指数 exponent 大于零时，如果当前指数位为 1，则将结果 result 与当前底数 base 相乘，并将结果取模 modulus。
然后，指数 exponent 右移一位（相当于除以 2），底数 base 进行平方运算并取模 modulus。
最后，返回计算得到的结果 result。

主函数：
```C
int main() {
    long long int a, b, c, p;
    scanf("%lld%lld%lld", &a, &b, &c);
    scanf("%lld", &p);
    // printf("%d %d %d %d\n", a, b, c, p);

    long long int exponent = power_mod(b, c, p-1);
    long long int result = power_mod(a, exponent, p);

    printf("%lld\n", result);
}
```
在主函数中，我们首先定义了变量 a、b、c 和 p，用于存储输入的正整数值。
使用 scanf 函数按照格式读取输入的数据，并将其存储在相应的变量中。
调用 power_mod 函数两次，先计算指数幂 exponent，然后计算最终结果 result。
最后，使用 printf 函数输出计算得到的结果。


在进行计算之前，我们对输入数据进行了合理性判断，特别是检查了底数 base 是否能整除模数 modulus，如果可以整除则直接返回 0。这种判断可以避免计算过程中出现除以零的错误。

另外，为了处理较大的整数和避免结果溢出，将数据类型 long long int 用于存储变量和计算结果。否则，可能由于最后结果溢出而导致 Wrong Answer。

模幂算法的时间复杂度为 $O(log(exponent))$，其中 exponent 是指数的大小。

在模幂算法中，指数 exponent 的二进制表示形式决定了循环迭代的次数。对于 n 位的二进制指数，循环迭代的次数最多为 n。

在每次迭代中，进行的操作包括对指数的位进行检查、底数的平方运算和取模运算。这些操作的时间复杂度都是 O(1)。

因此，整个模幂算法的时间复杂度主要由迭代次数决定，即 $O(log(exponent))$。