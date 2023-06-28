#include<bits/stdc++.h>
#define MAXN 100005
#define lowbit(x) ((x) & (-x))

using namespace std;

int n, k;
int a[MAXN];
int dp[2][MAXN];
int tree[MAXN];
int b[MAXN];

inline void update(int i, int x)
{
    for (int pos = i; pos <= n; pos += lowbit(pos))
        tree[pos] = (tree[pos] + x)%1000;
}

inline int query(int n)
{
    int ans = 0;
    for (int pos = n; pos; pos -= lowbit(pos))
        ans = (ans + tree[pos])%1000;
    return ans;
}

void dis() {
    for(int i = 1; i <= n; i++) 
        b[i] = a[i];
    sort(b+1, b+1+n);
    int len=unique(b+1, b+1+n)-(b+1);
    for(int i = 1; i <= n; i++) 
        a[i] = lower_bound(b+1, b+1+len, a[i])-b;
}

int main() {
    scanf("%d", &n);
    scanf("%d", &k);

    if(k > n) {
        printf("0\n");
        return 0;
    }

    for(int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
    }
    dis();
    int r=1, l=0;
    for(int i = 1; i <= n; i++) 
        dp[0][i] = 1;
    for(int t = 2; t <= k; t++) {
        for(int i = 1; i <= n; i++) {
            dp[r][i]=query(a[i]-1);
            update(a[i], dp[l][i]);
        }
        for(int i = 1; i<= n; i++) 
            tree[i]=0;
        r = 1-r;
        l = 1-l;
    }

    // 
    int ans = 0;
    for(int i = 1; i <= n; i++)
        ans = (ans+dp[l][i])%1000;
    printf("%d\n", ans);
    return 0;
}