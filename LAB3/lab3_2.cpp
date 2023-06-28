#include<bits/stdc++.h>

#define MAX 1000005

using namespace std;

int father[MAX], child[MAX], num[MAX], cover[MAX], depth[MAX], dfs[MAX];
// int visit[MAX];
int n, r;

void show() {
    printf("FATHER:\n");
    for(int i = 0; i <= n; i++) {
        printf("%d ",father[i]);
    }
    printf("\n");
    printf("CHILD:\n");
    for(int i = 0; i <= n; i++) {
        printf("%d ",child[i]);
    }
    printf("\n");
    printf("NUM:\n");
    for(int i = 0; i <= n; i++) {
        printf("%d ", num[i]);
    }
    printf("\n");
    printf("HEIGHT:\n");
    for(int i = 0; i <= n; i++) {
        printf("%d ", depth[i]);
    }
    printf("\n");
}

int findMaxDepth() {
    int max = -1;
    for(int i = 1; i <= n; i++) {
        // if(depth[i] > max)
        //     max = depth;
        max = (depth[i] > max ? depth[i] : max);
    }
    // printf("maxDepth: %d\n", max);
    return max;
}

void DFS() {
    dfs[1] = 1;
    int start = 0, end = 1;
    while(start != end) {
        start ++;
        for(int i = num[dfs[start]-1]+1; i <= num[dfs[start]]; i++) {
            dfs[++end] = child[i];
        }
        // if(visit[dfs[start]] == 0) {
        //     visit[dfs[start]] = 1;
            // for(int i = num[dfs[start]-1]+1; i <= num[dfs[start]]; i++) {
            //     dfs[++end] = child[i];
            // }
        // }
    }
    // for(int i = 0; i <= n; i++) {
    //     printf("%d ", dfs[i]);
    // }
    // printf("\n");
}

int findRParent(int a, int r) {
    if(a < 0) 
        return -1;
    if(r == 0)
        return a;
    return findRParent(father[a], r-1);
}

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
    // for(int i = 1; i <= n; i++) {
    //     printf("%d ", cover[i]);
    // }
    // printf("\n");
}

int main() {
    scanf("%d", &n);
    scanf("%d", &r);

    memset(father, -1, sizeof(father));
    memset(child, -1, sizeof(child));
    memset(num, 0, sizeof(num));
    memset(cover, 0, sizeof(cover));
    memset(depth, 0, sizeof(depth));
    // memset(visit, 0, sizeof(visit));
    // depth[1] = 1;

    int m;
    int ind = 0, count = 0;
    while(1) {
        scanf("%d", &m);
        ind++;
        int a;
        num[ind] = num[ind] + num[ind-1];
        for(int i = 0; i < m; i++) {
            scanf("%d", &a);
            father[a] = ind;
            child[++num[ind]] = a;
            depth[a] = depth[ind] + 1;
            count++;
        }
        if(count == n-1) break;
    }
    // show();
    if(findMaxDepth() <= r) {
        printf("1\n");
        return 0;
    }
    DFS();

    count = 0 ;
    for(int i = n; i > 0; i--) {
        if(cover[dfs[i]] == 0) {
            // printf("dfs[i]=%d  cover[dfs[i]]=%d count=%d\n", dfs[i], cover[dfs[i]], count);
            int a = findRParent(dfs[i], r);
            if(a < 0) {
                count++;
                printf("%d\n", count);
                return 0;
            }
            count++;
            update(a, r);
            // printf("%d ", a);
        }
    }

    // printf("COUNT:\n");
    printf("%d\n", count);
    return 0;
}