#include<bits/stdc++.h>

#define MAX 605
#define INF 268435455

using namespace std;
int n, m;
int dist[MAX][MAX];

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

void show() {
    int i, j;
    for(i = 1; i <= n; i++) {
        for(j = 1; j <= n; j++) {
            printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
}

int main() {
    scanf("%d %d", &n, &m);
    int u, v, w;
    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= n; j++) {
            dist[i][j] = INF;
        }
        dist[i][i] = 0;
    }

    for(int i = 0; i < m; i++) {
        scanf("%d %d %d", &u, &v, &w);
        dist[u][v] = w;
    }    

    // show();
    floydWarshall();
    // show();
    for(int i = 1; i <= n; i++) {
        if(dist[i][i] < 0) {
            printf("negative-weight cycle\n");
            return 0;
        }
    }
    int q;
    scanf("%d", &q);
    for(int i = 0; i < q; i++) {
        scanf("%d %d", &u, &v);
        printf("%d\n", dist[u][v]);
    }
    return 0;
}