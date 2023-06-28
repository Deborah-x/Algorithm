#include <stdio.h>

int n;

// Pass the front 5 examples but fail the last 5 for `Runtime Error`

void coverChild(int E[][n+10], int *V, int r, int a) {
    if(r == 0) return;
    for(int i = 1; i < n+1; i++) {
        if(E[a][i] == 1) {
            V[i] = 1;
            coverChild(E, V, r-1, i);
        }
    }
}

void update(int E[][n+10], int *V, int r, int a) {
    if(r == 0) return;
    // int key = r;
    coverChild(E, V, r, a);
    int i;
    for(i = 1; i < n+1; i++) {
        if(E[i][a] == 1) break;
    }
    if(i == n+1) {
        return;
    }
    V[i] = 1;
    // r--;
    coverChild(E, V, r-1, i);
    update(E, V, r-1, i);
}

int back(int E[][n+10], int *V, int r, int value) {
    int i, j;
    for(i = r; i > 0; i--) {
        for(j = 1; j < n+1; j++) {
            if(E[j][value] == 1) {
                value = j;
                break;
            }
        }
        if(j == n+1) {
            return 0;
        }
    }
    // printf("value = %d\n", value);

    return value;
}

int main() {
    int i, j, k;
    int r;
    scanf("%d", &n);
    scanf("%d", &r);
    
    int m;
    int ind = 0, count = 0;
    int E[n+10][n+10], V[n+10];
    for(i = 1; i < n+1; i++) {
        for(j = 1; j < n+1; j++) {
            E[i][j] = 0;
        }
    }
    
    while(1) {
        scanf("%d", &m);
        ind++;
        int a;
        for(i = 0; i < m; i++) {
            scanf("%d", &a);
            E[ind][a] = 1;
            // printf("ind=%d a=%d  ", ind, a);s
            count++;
        }
        if(count == n-1) break;

    }
    // for(int i = 1; i < n+1; i++) {
    //     for(int j = 1; j < n+1; j++) {
    //         printf("%d ", E[i][j]);
    //     }
    //     printf("\n");
    // }

    for(i = 1; i < n+1; i++) {
        V[i] = 0;
    }

    int set[n+10];
    set[1] = 1;
    int start = 0, end  = 1;
    while(start != end) {
        start++;
        if(V[set[start]] == 0) {
            V[set[start]] = 1;
            // set[end++] = start;
            for(int i = 1; i < n+1; i ++) {
                if(E[set[start]][i] == 1) {
                    set[++end] = i;
                }
            }
        }
        // printf("start:%d end:%d\n", start, end);
    }

    // for(int j = 1; j < n+1; j++) {
    //     printf("%d ", set[j]);
    // }
    for(i = 0; i < n + 1; i++) {
        V[i] = 0;
    }
    int cover[n+10];
    int post = 0;
    // printf("end = %d\n", set[end]);
    int value = set[end];
    value = back(E, V, r, value);
    if(value == 0) {
        printf("1\n");
        return 0;
    }
    V[value] = 1;
    update(E, V, r, value);
    // for(int i = 1; i < n+1; i++) {
    //     printf("%d ", V[i]);
    // }
    // printf("\n");
    post++;
    cover[post] = value;

    for(i = n; i > 0; i--) {
        if(V[i] == 0) {
            value = back(E, V, r, i);
            if(value == 0) {
                post++;
                cover[post] = 1;
                printf("%d\n", post);
                return 0;
            }
            V[value] = 1;
            post++;
            cover[post] = value;
            update(E, V, r, value);
        }
        // for(int i = 1; i < n+1; i++) {
        //     printf("%d ", V[i]);
        // }
        // printf("\n");
    }
    printf("%d\n", post);
    return 0;
}