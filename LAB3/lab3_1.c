#include <stdio.h>

int shortestInterval(int *array, int n, int K) {
    int left = 0, right = 0;
    int sum = array[0], min_len = n + 1;

    while(right < n) {
        if(sum < K) {
            right ++;
            if(right < n) {
                sum += array[right];
            }
        }
        else {
            min_len = min_len < right - left + 1 ? min_len : right - left + 1;
            sum -= array[left];
            left++;
        }
    }
    return min_len;
}

// int shortestInterval(int *array, int n, int K) {
//     int min_len = n + 1;

//     for (int len = 1; len <= n; len++) {
//         int left = 0, right = 0;
//         int sum = 0;
//         for(int j = 0; j < len; j++) {
//             sum += array[j];            
//         }
//         right = len;
//         while(right <= n) {
//             // printf("sum:%d\n", sum);
//             if(sum >= K) return len;
//             sum += array[right++];
//             sum -= array[left++];
//         }
//     }
    
//     return min_len;
// }


int main() {
    int n, K;
    scanf("%d", &n);
    scanf("%d", &K);
    // printf("%d %d", n, K);
    int array[n+1];
    int sum = 0;
    for(int i = 0; i < n; i++) {
        scanf("%d", &array[i]);
        sum += array[i];
    }
    if(sum < K) {
        printf("0\n");
        return 0;
    }
    // for(int i = 0; i < n; i++) {
    //     printf("%d ", array[i]);
    // }
    // int start = 0, end = n-1;
    // while(start != end) {
    //     if((sum-array[start] > K) || (sum-array[end] > K)) {
    //         if(array[start] < array[end]) {
    //             sum -= array[start];
    //             start++;
    //         }
    //         else {
    //             sum -= array[end];
    //             end--;
    //         }
    //     }
    //     else {
    //         break;
    //     }
    // }
    // printf("%d\n", end-start+1);
    int result;
    result = shortestInterval(array, n, K);
    printf("%d\n", result);
    return 0;
}