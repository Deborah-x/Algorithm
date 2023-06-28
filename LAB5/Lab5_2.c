#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int n1, m1;
int A[1305][1305];
int n2, m2;
int B[3005][20005];
int ha = 0, hb = 0;
const int p1 = 19, p2 = 881;

void matching() {
    for(int i = 1; i <= n2; i++) {
        for(int j = 1; j <= m2; j++) 
            B[i][j] = B[i][j] + p1*B[i][j-1] + p2*B[i-1][j] - p1*p2*B[i-1][j-1];
    }

    int t1 = pow(p1, m1), t2 = pow(p2,n1);
    for(int i = n1; i <= n2; i++) {
        for(int j = m1; j <= m2; j++) {
            hb = B[i][j] - t1*B[i][j-m1] - t2*B[i-n1][j] + t1*t2*B[i-n1][j-m1];
            if(hb == ha)
                printf("%d %d\n", i-n1, j-m1);
        }
    }
}

int main() {
    char ch;
    scanf("%d %d", &n1, &m1);
    for(int i = 1; i <= n1; i++) {
        getchar();
        for(int j = 1; j <= m1; j++) {
            scanf("%c", &ch);
            A[i][j] = ch - 'a' + 1;
        }
    }

    // for(int i = 1; i <= n1; i++) {
    //     for(int j = 1; j <= m1; j++)
    //         printf("%d\t", A[i][j]);
    //     printf("\n");
    // }

    for(int i = 1; i <= n1; i++) {
        for(int j = 1; j <= m1; j++)
            A[i][j] = A[i][j] + p1*A[i][j-1] + p2*A[i-1][j] - p1*p2*A[i-1][j-1];
    }
    ha = A[n1][m1];

    scanf("%d %d", &n2, &m2);
    for(int i = 1; i <= n2; i++) {
        getchar();
        for(int j = 1; j <= m2; j++) {
            scanf("%c", &ch);
            B[i][j] = ch - 'a' + 1;
        }
    }

    // for(int i = 1; i <= n2; i++) {
    //     for(int j = 1; j <= m2; j++)
    //         printf("%d\t", B[i][j]);
    //     printf("\n");
    // }

    matching();
    return 0;
}