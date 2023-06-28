#include <stdio.h>
#include <stdlib.h>

#define MAX 150001

typedef struct Edge {
    int v;
    struct Edge* next;
} Edge;

int n, m, q;
int dfn[MAX], low[MAX], color[MAX], instack[MAX];
int dfn_cnt, color_cnt;
Edge* adj[MAX];
int stack[MAX], top;

void init() {
    int i;
    for (i = 1; i <= n; i++) {
        adj[i] = NULL;
        dfn[i] = low[i] = color[i] = instack[i] = 0;
    }
    dfn_cnt = color_cnt = top = 0;
}

void addEdge(int u, int v) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->v = v;
    newEdge->next = adj[u];
    adj[u] = newEdge;
}

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

int bfs(int u, int v) {
    int* queue = (int*)malloc(sizeof(int) * (n + 1));
    int front = 0, rear = 0;
    int* visited = (int*)malloc(sizeof(int) * (n + 1));
    int i;

    for (i = 1; i <= n; i++) {
        visited[i] = 0;
    }

    queue[rear++] = u;
    visited[u] = 1;

    while (front < rear) {
        int curr = queue[front++];
        if (curr == v) {
            free(queue);
            free(visited);
            return 1;
        }

        Edge* e;
        for (e = adj[curr]; e != NULL; e = e->next) {
            int next = e->v;
            if (!visited[next]) {
                queue[rear++] = next;
                visited[next] = 1;
            }
        }
    }

    free(queue);
    free(visited);
    return 0;
}

int main() {
    scanf("%d %d", &n, &m);
    init();

    int i, u, v;
    for (i = 0; i < m; i++) {
        scanf("%d %d", &u, &v);
        addEdge(u, v);
    }

    for (u = 1; u <= n; u++) {
        if (dfn[u] == 0) {
            tarjan(u);
        }
    }

    scanf("%d", &q);
    int a;
    for (i = 0; i < q; i++) {
        scanf("%d %d %d", &a, &u, &v);
        if (a == 0) {
            if (color[u] == color[v]) {
                printf("1\n");
            } else {
                int result = bfs(u, v);
                printf("%d\n", result);
            }
        } else if (a == 1) {
            addEdge(u, v);
        }
    }

    return 0;
}
