#include<iostream>
#include<queue>
#include<cstring>
#define N 605
#define M 42005
const int inf=0x7fffffff;

using namespace std;

int n, m;
int r[N][N];
// int pre[N];
int d[N];

bool BFS(int s, int t) {
    int p;
    queue<int> q;
	memset(d, -1, sizeof(d));
    // memset(pre, -1, sizeof(pre));
    // pre[s] = s;
    d[s] = 0;
    q.push(s);
    while(!q.empty()) {
        p = q.front();
        q.pop();
        for(int i = 1; i <= n; i++) {
            if(r[p][i] > 0 && d[i] == -1) {
                // pre[i] = p;
                d[i] = d[p] + 1;
                if(i == t)
                    return true;
                q.push(i);
            }
        }
    }
    return false;
}

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

int main() {
    scanf("%d%d", &n, &m);
    int u, v, w;
    memset(r, 0, sizeof(r));
    for(int i = 0; i < m; i++) {
        scanf("%d%d%d", &u, &v, &w);
        r[u][v] = w;
    }

    scanf("%d%d", &u, &v);
	// BFS(u, v);
	// for(int i = 1; i <= n; i++)
	// 	printf("%d ", d[i]);

    printf("%d\n", Dinic(u, v));
    return 0;
}