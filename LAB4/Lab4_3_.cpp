#include <iostream>
#include <cstring>

using namespace std;

#define MAXLEN 605
#define INF 268435455

int n, m;
int dist[MAXLEN][MAXLEN];
int tot, ver[43010], Next[43010], head[43010], weight[43010];

void add(int x, int y, int w) {
    ver[++tot] = y;
    weight[tot] = w;
    Next[tot] = head[x];
    head[x] = tot;
}

int d[MAXLEN];
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

bool mark[MAXLEN];
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

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            dist[i][j] = INF;
        }
        dist[i][i] = 0;
    }
    for (int i = 1; i <= m; i++) {
        int x, y, w;
        scanf("%d%d%d", &x, &y, &w);
        add(x, y, w);
    }
    if (Bellman()) 
        return 0;
    for (int x = 1; x <= n; x++) {
        for (int v = head[x]; v; v = Next[v]) {
            int y = ver[v];
            weight[v] += d[x] - d[y];
        }
    }
    for (int x = 1; x <= n; x++) 
        Dijkstra(x);
    int q;
    scanf("%d", &q);
    while (q--) {
        int u, v;
        scanf("%d%d", &u, &v);
        if (dist[u][v] == INF)
            puts("268435455");
        else
            printf("%d\n", dist[u][v] + d[v] - d[u]);
    }
    return 0;
}
