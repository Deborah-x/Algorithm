#include<cstdio>
#include<cstring>
#include<algorithm>
#define Inf 0x3f3f3f3f

using namespace std;
const int N = 5001;
int G[N][N];        // 存放原图 
int used[N][N];     // 记录i-j这条边是否在最小生成树上 
int path[N][N];     // i-j之间最大的边长 
int vis[N], dis[N], per[N];
int n, m;

void init() {
	memset(used, 0, sizeof(used));
	memset(path, 0, sizeof(path));
	memset(G, Inf, sizeof(G));
}

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

int main() {
    scanf("%d%d", &n, &m);

    init();
    for(int i  =  1; i <= m; i++){
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        G[u][v] = G[v][u] = w;
    }
    int temp = Prim(1);
    int ans = SecPrim(temp);
    // printf("最小生成树： %d\n", temp);
    // printf("次小生成树： %d\n", ans);
    printf("%d\n", temp);
    printf("%d\n", ans);

	return 0;
}