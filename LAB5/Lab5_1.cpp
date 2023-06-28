#include<iostream>
#include<queue>
#include<cstring>
#define N 605
#define M 42005
const int inf=0x7fffffff;

using namespace std;

int n, m;
int r[N][N];
bool visit[N];
int pre[N];

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

int main() {
    scanf("%d%d", &n, &m);
    int u, v, w;
    memset(r, 0, sizeof(r));
    for(int i = 0; i < m; i++) {
        scanf("%d%d%d", &u, &v, &w);
        // printf("%d %d %d\n", u, v, w);
        r[u][v] = w;
    }

    // for(int i = 1; i <= n; i++) {
    //     for(int j = 1; j <= n; j++) {
    //         printf("%d ", r[i][j]);
    //     }
    //     printf("\n");
    // }

    scanf("%d%d", &u, &v);
    printf("%d\n", Edmonds_Karp(u, v));
    return 0;
}