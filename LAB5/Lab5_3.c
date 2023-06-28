#include <stdio.h>

// 模幂算法
long long int power_mod(long long int base, long long int exponent, long long int modulus) {
    // printf("base:%d,exponent:%d,modulus:%d\n", base, exponent, modulus);

    if(base % modulus == 0)
        return 0;

    long long int result = 1;
    base = base % modulus;

    while(exponent > 0) {
        if(exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    // printf("result:%d\n", result);
    return result;
}

int main() {
    long long int a, b, c, p;
    scanf("%lld%lld%lld", &a, &b, &c);
    scanf("%lld", &p);
    // printf("%d %d %d %d\n", a, b, c, p);

    long long int exponent = power_mod(b, c, p-1);
    long long int result = power_mod(a, exponent, p);

    printf("%lld\n", result);
    return 0;
}