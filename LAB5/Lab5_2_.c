#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n1, m1;
int n2, m2;

char A[1305][1305];
char B[3005][20005];

int match(int row, int col) {
    for(int i = 0; i < n1; i++) {
        for(int j = 0; j < m1; j++) {
            if(A[i][j] != B[row + i][col + j]) {
                return 0;
            }
        }
    }
    return 1;
}

void findSubBlock() {
    for(int i = 0; i <= n2 - n1; i++) {
        for(int j = 0; j <= m2 - m1; j++) {
            if(match(i, j)) {
                printf("%d %d\n", i, j);
            }
        }
    }
}

int main() {
    scanf("%d %d", &n1, &m1);
    for(int i = 0; i < n1; i++) {
        scanf("%s", A[i]);
    }
    // for(int i = 1; i <= n1; i++) {
    //     for(int j = 1; j <= m1; j++)
    //         printf("%d\t", A[i][j]);
    //     printf("\n");
    // }
    scanf("%d %d", &n2, &m2);
    for(int i = 0; i < n2; i++) {
        scanf("%s", B[i]);
    }

    findSubBlock();

    return 0;
}