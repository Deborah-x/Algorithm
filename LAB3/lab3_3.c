#include <stdio.h>

int n, k;

int a[100005];
int dp[100005][105];
// dp[i][j] 表示到第 i 个元素，长度为 j 的递增子序列个数


// void show() {
//     for(int i = 1; i <= n; i++) {
//         for(int j = 1; j <= k; j++) {
//             printf("%d ", dp[i][j]);
//         }
//         printf("\n");
//     }
// }

int main() {
    scanf("%d", &n);
    scanf("%d", &k);

    if(k > n) {
        printf("0\n");
        return 0;
    }

    for(int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        dp[i][1] = 1;
    }
    // for(int i = 1; i <= n; i++) {
    //     for(int j = 1; j <= k; j++) {
    //         dp[i][j] = 0;
    //     }
    // }
    
    for(int i = 1; i <= n; i++) {
        for(int j = 2; j <= k; j++) {
            for(int t = 1; t < i; t++) {
                if(a[i] > a[t]) 
                    dp[i][j] = (dp[i][j] + dp[t][j-1]) % 1000 ;
            }
        }
    }
    // show();
    int ans = 0;
    for(int i = 1; i <= n; i++)
        ans = (ans+dp[i][k])%1000;
    printf("%d\n", ans);
    // for(int i = 0; i < n; i++) {
    //     printf("%d  ", a[i]);
    // }
    return 0;
}