# Lab 4-PB20000178李笑
## Q1: 连通性查询
连通性查询最简单的实现办法是使用邻接表或邻接矩阵存储图信息，然后每次查询通过 DFS 或 BFS 遍历判断是否可达，显然算法的时间复杂度为 $O(nq)$，其中 n 是图的顶点数，q 是查询操作的次数。算法正确性也易得，但是在这题中直接使用遍历会导致部分样例超时，因此针对算法时间复杂度的优化我采用了群友提供的 Tarjian 算法来缩点。

Tarjian 算法可以将一个有环图中的连通块缩成一个点(同一个连通块染上相同颜色)，使其变成一个 DAG (有向无环图)。具体实现如下：

```C
void tarjan(int u) {
    int v;
    dfn[u] = low[u] = ++dfn_cnt;
    stack[++top] = u;
    instack[u] = 1;

    Edge* e;
    for (e = adj[u]; e != NULL; e = e->next) {
        v = e->v;
        if (dfn[v] == 0) {
            tarjan(v);
            if (low[v] < low[u])
                low[u] = low[v];
        } else if (instack[v] && dfn[v] < low[u]) {
            low[u] = dfn[v];
        }
    }

    if (dfn[u] == low[u]) {
        color_cnt++;
        do {
            v = stack[top--];
            instack[v] = 0;
            color[v] = color_cnt;
        } while (v != u);
    }
}
```

在每次查询时，先判断需要查询连通性的两个点是否在一个强连通分量中，若在，直接输出 `1`，否则，进行 BFS 遍历，根据可达性输出对应值。Tarjian 算法的时间复杂度为 $O(n)$，但它能将同一个强连通分量里的查询时间减少到 $O(1)$，有效减少原时间复杂度中 q 的大小。

## Q2: 最小和次小生成树
在作业 4 中我们已经提出了最小和次小生成树算法，并证明了算法正确性。在这里我们使用 Kruskal 算法求最小生成树，具体实现如下：

```C++
// 初始化并查集
void UFset(){
	for(i = 1; i <= n; i++) 
		parent[i] = -1;
}

// 查找 i 的根
int find(int i){
	int temp;
	// 查找位置
	for(temp = i; parent[temp] >= 0; temp = parent[temp]);
	// 压缩路径
	while(temp != i){
		int t = parent[i];
		parent[i] = temp;
		i = t;
	}
	return temp;
}

// 合并两个元素a,b
void merge(int a,int b){
	int r1 = find(a);
	int r2 = find(b);
	int tmp = parent[r1] + parent[r2]; // 两个集合节点数的和
	if(parent[r1] > parent[r2]){
		parent[r1] = r2;
		parent[r2] = tmp;
	}else{
		parent[r2] = r1;
		parent[r1] = tmp;
	}
}

void kruskal(){
	int sumWeight = 0;
	int num = 0;
	int u,v;
	UFset();
	init();

	for(int i = 0; i < m; i++)
	{
		u = edges[i].u;
		v = edges[i].v;
 
		if(find(u) != find(v)){ // u 和 v 不在一个集合
			sumWeight += edges[i].w;
			num ++;
			merge(u, v); // 把这两个边加入一个集合。
			visit[i] = 1;
		}
	}
	printf("%d\n", sumWeight);
}
```
算法时间复杂度为 $O(ElgV)$。其中使用到了并查集，使得能在 $O(1)$ 时间查询两个结点是否属于同一颗树（即查询添加边是否安全）。在算法中我们维护了一个 `visit[]`数组用于存储某条边是否在最小生成树中。

次小生成树与最小生成树只有一条边的差别，我们通过每次“遮住”一条在最小生成树中的边，然后利用其他边来生成最小生成树方法，在 $|V|-1$个新生成的最小生成树中选择最小的即为次小生成树。
```C++
void subMinTree() {
	int sumWeight;
	int Weight = 268435455;
	int u,v;
	for(int j = 0; j < m; j++) {
		if(visit[j] == 1) {
			sumWeight = 0;
			UFset();
			for(int i = 0; i < m; i++)
			{
				if(i == j) 
					continue;

				u = edges[i].u;
				v = edges[i].v;
		
				if(find(u) != find(v)){
					sumWeight += edges[i].w;
					merge(u, v);
				}
			}
			Weight = min(Weight, sumWeight);
		}
	}

	printf("%d\n", Weight);
}
```
这个实现方法建立在 Kruskal 算法求最小生成树的基础上，算法实现简单，但是时间复杂度为 $O(EVlgV)$，在最后两个样例上遗憾超时。

然后我采用作业中的改进算法，每次枚举一个不在最小生成树中的边，然后从新生成的环路中去掉一条（除加入边外）权重最大的边。枚举过程中得到的总权值和最小的树即为次小生成树。

最小生成树和次小生成树的具体实现如下：
```C++
int Prim(int s){
	memset(vis, 0, sizeof(vis));
	int sum = 0;
	for(int i = 1; i <= n; i++) {
		dis[i] = G[s][i];
		per[i] = s;
	}
	vis[s] = 1;
	for(int i = 1; i < n; i++) {
		int mint = Inf;
		int u = s;
		for(int j = 1; j <= n; j++) {
			if(!vis[j] && dis[j] < mint) {
				mint = dis[j];
				u = j;
			}
		}
		vis[u] = 1;
		sum += mint;
		used[u][per[u]] = used[per[u]][u] = 1;
		for(int j = 1; j <= n; j++) {
			if(vis[j] && j != u) //j到u路径上的最大值 
			  	path[j][u] = path[u][j] = max(path[j][per[u]], dis[u]);
			if(!vis[j]) {
				if(dis[j] > G[u][j]) {
					dis[j] = G[u][j];
					per[j] = u;
				}
			}
		}
	}
	return sum;
}

int SecPrim(int x){
	// printf("%d\n", x);
	int ans = Inf;
	for(int i = 1; i <= n; i++)
		for(int j = i+1; j <= n; j++)
		if(!used[i][j]) //去掉i, j 之间的最大值，加上i-j这条边 
			ans = min(ans, x-path[i][j]+G[i][j]);
	return ans;
}
```
算法时间复杂度为 $O(V^2)$。


## Q3: 顶点距离
### Floyd-Warshall 算法
图的信息使用邻接矩阵存储。Folyd-Warshall 算法是一种动态规划算法，从短到长计算两点之间的最短距离。三个 for 循环导致时间复杂度为 $O(n^3)$
```C++
void floydWarshall() {
    int i, j, k;
    for(k = 1; k <= n; k++) {
        for(i = 1; i <= n; i++) {
            for(j = 1; j <= n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
}
```

### Johnson 算法
```C++
bool Bellman() {
    for (int i = 1; i <= n; i++) 
        add(0, i, 0);
    for (int i = 1; i <= n; i++) 
        d[i] = INF;
    bool flag;
    for (int t = 1; t <= n+2; t++) {
        flag = false;
        for (int x = 0; x <= n; x++) {
            if (d[x] == INF) 
                continue;
            for (int v = head[x]; v; v = Next[v]) {
                int y = ver[v], w = weight[v];
                if (d[y] > d[x] + w) {
                    d[y] = d[x] + w;
                    flag = true;
                }
            }
        }
        if (!flag) break;
    }
    if (flag) {
        puts("negative-weight cycle");
        return true;
    }
    return false;
}

void Dijkstra(int s) {
    memset(mark, 0, sizeof(mark));
    for (int t = 1; t <= n; t++) {
        int target = 0, mind = INF;
        for (int i = 1; i <= n; i++) {
            if (!mark[i] && dist[s][i] < mind) {
                mind = dist[s][i];
                target = i;
            }
        }
        if (target == 0) 
            return;
        mark[target] = true;
        for (int v = head[target]; v; v = Next[v]) {
            int y = ver[v], w = weight[v];
            dist[s][y] = min(dist[s][y], dist[s][target] + w);
        }
    }
}
```
`Bellman()` 函数实现了 Bellman-ford 算法，以查找从源顶点 0 到所有其它顶点的最短路径。它初始化所有顶点的距离数组 d 为无穷大 INF，除了源顶点设置为 0 。然后，它反复放松边缘以找到最短路径，直到不能再进行更新或直到达到最大迭代次数 n+2。如果在迭代过程中检测到负权重循环，则该函数打印“负权重循环”并返回 true。

Bellman-Ford 算法完成后，代码根据上一步计算出的距离 d 调整图中边的权重。这一步对于从图中删除任何负权重是必要的，这是 Dijkstra 算法所要求的。

`Dijkstra()` 函数实现了 Dijkstra 算法，以查找从给定源顶点到所有其它顶点的最短路径。它使用贪婪的方法，迭代地从未标记的顶点中选择与源距离最小的顶点。然后，如果找到更短的路径，它会更新相邻顶点的距离。

算法的时间复杂度由 `Bellman()` 和 `Dijkstra()` 两部分决定，为 $O(nm + n(n+m)logn)$

### 算法比较
Floyd算法：

* Floyd算法是一种动态规划算法，用于计算图中所有顶点对之间的最短路径。
* Floyd算法使用一个二维数组来存储任意两个顶点之间的最短路径长度。通过不断更新这个数组，最终得到所有顶点对之间的最短路径长度。
* Floyd算法适用于解决稠密图中的最短路径问题，即图中边的数量接近于顶点数量的平方。
* Floyd算法的时间复杂度是O(n^3)，其中n是顶点的数量。因此，对于较大的图，算法的运行时间可能会较长。
* __优点__：Floyd算法适用于有向图和无向图，并且可以处理负权边的情况。它能够找到所有顶点对之间的最短路径，并且可以同时计算最短路径的长度。
* __缺点__：Floyd算法的时间复杂度较高，在大型图上的运行时间可能会很长。此外，Floyd算法需要额外的空间来存储所有顶点对之间的最短路径长度，因此对于大型图而言，空间复杂度也较高。

Johnson算法：

* Johnson算法是一种利用Bellman-Ford和Dijkstra算法的优化算法，用于计算图中所有顶点对之间的最短路径。
* Johnson算法首先通过添加一个额外的顶点和边来对图进行变换，消除了负权边的影响。
* 然后，Johnson算法使用Bellman-Ford算法计算从额外的顶点到所有其他顶点的最短路径，并通过调整边权重来消除负权重。
* 最后，对于每个顶点，使用Dijkstra算法计算从该顶点到其他所有顶点的最短路径。
* Johnson算法适用于解决稀疏图中的最短路径问题，即图中边的数量远小于顶点数量的平方。
* Johnson算法的时间复杂度是 $O(nm + n(n+m)logn)$，其中n是顶点的数量，m是边的数量。
* __优点__：Johnson算法适用于有向图和无向图，并且可以处理负权边的情况。相对于Floyd算法，Johnson算法在稀疏图上的运行时间较短。
* __缺点__：Johnson算法需要使用Bellman-Ford和Dijkstra算法，因此需要额外的代码实现。此外，对较大的稠密图，Johnson算法的运行时间也会较长。