#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

#define MAX_ITER    20
#define MAX_TS      9

#define GOLD_RATE   0.6180339887

typedef struct{
	int length;
	int *Bucket1;
	int *Bucket2;
	int TS[MAX_TS];    // temporary storage
	int prime;
}CuckooHash;

int isPrime(int n) {
    for(int i = 2; i < sqrt(n); i++) {
        if(n % i == 0) return 0;
    } 
    return 1;
}

int getPrime(int m) {
    while(!isPrime(m)) {
        m--;
    }
    return m;
}

int hash1(CuckooHash *ht, int key) {
    // return (key&(0b110011001100110011001100))%ht->prime;
    // return (int)(key / 2) % ht->prime;
    return ((3507*key)&16777215)%ht->prime;
    // return ((0b11011011011011011011)*key)&(0b111111111111111111111111)%ht->prime;
}

int hash2(CuckooHash *ht, int key) {
    // return (int)(3.14*key+0.618) % ht->length;
    // return (int)(sqrt(key*key + 1)) % ht->length;
    // return (int)(key / 3) % ht->length;
    double s = key*GOLD_RATE - floor(key*GOLD_RATE);
	return floor(ht->length*s);
    // return (int)(key*key + 1.9*key + 2.2) % ht->length;
}

void initHashTable(CuckooHash *ht, int m) {
    ht->length = m;
    ht->prime = getPrime(m);
	ht->Bucket1 = (int*)malloc(m*sizeof(int));
	ht->Bucket2 = (int*)malloc(m*sizeof(int));
	memset(ht->TS, -1, MAX_TS*sizeof(int));
	memset(ht->Bucket1, -1, m*sizeof(int));
	memset(ht->Bucket2, -1, m*sizeof(int));
}

int lookUpKey(CuckooHash *ht, int key) {
    if(ht->Bucket1[hash1(ht, key)] == key) {
        return 1;
    }
    else if(ht->Bucket2[hash2(ht, key)] == key) {
        return 1;
    }
    else {
        for(int i = 0; i < MAX_TS; i++) {
            if(ht->TS[i] == key) return 1;
        }
    }
    return 0;
}

void insertKey(CuckooHash *ht, int key) {
    if(lookUpKey(ht, key)) return;
    int iter = 0;
    int index;
    int temp;

    for(int i = 0; i < MAX_ITER; i++) {
        index = hash1(ht, key);
        if(ht->Bucket1[index] == -1) {
            ht->Bucket1[index] = key;
            return;
        }
        temp = ht->Bucket1[index]; ht->Bucket1[index] = key; key = temp;
        index = hash2(ht, key);
        if(ht->Bucket2[index] == -1) {
            ht->Bucket2[index] = key;
            return;
        }
        temp = ht->Bucket2[index]; ht->Bucket2[index] = key; key = temp;
        // int r = rand();
        // if(r % 2 == 0) {
        //     temp = ht->Bucket2[index]; ht->Bucket2[index] = key; key = temp;
        // }
        // else {
        //     temp = ht->Bucket1[index]; ht->Bucket1[index] = key; key = temp;
        // }
    }

    for(int i = 0; i < MAX_TS; i++) {
        if(ht->TS[i] == -1) {
            ht->TS[i] = key;
            return;
        }
    }

    return;
}

void deleteKey(CuckooHash *ht, int key) {
    int index = hash1(ht, key);
    if(ht->Bucket1[index] == key) {
        ht->Bucket1[index] = -1;
        return;
    }

    index = hash2(ht, key);
    if(ht->Bucket2[index] == key) {
        ht->Bucket2[index] = -1;
        return;
    }

    for(int i = 0; i < MAX_TS; i++) {
        if(ht->TS[i] == key) {
            ht->TS[i] = -1;
            return;
        }
    }
}

// void show(CuckooHash *ht) {
//     printf("H1:\n");
//     for(int i = 0; i < ht->length; i++) {
//         printf("%d\t", ht->Bucket1[i]);
//     }
//     printf("\nH2:\n");
//     for(int i = 0; i < ht->length; i++) {
//         printf("%d\t", ht->Bucket2[i]);
//     }
//     printf("\nTS: \n");
//     for(int i = 0; i < MAX_TS; i++) {
//         printf("%d\t", ht->TS[i]);
//     }
//     printf("\n");
//     return;
// }

// void littleSize(int m, int n) {
//     char op; 
//     int key;

//     // int P[n], post=0;

//     CuckooHash ht;
//     ht.length = 1.7*m;
//     initHashTable(&ht, ht.length);
//     // show(&ht);

//     for (int i = 0; i < n; i++) {
//         scanf(" %c%d", &op, &key);

//         if (op == 'L') {
//             // P[post++] = lookUpKey(&ht, key);
//             printf("%d\n", lookUpKey(&ht, key));
//         }
//         else if (op == 'I') {
//             insertKey(&ht, key);
//             // show(&ht);
//         }
//         else if (op == 'D') {
//             deleteKey(&ht, key);
//             // show(&ht);
//         }
//     }
//     return;
// }

int main(){
    int m, n;
    scanf("%d %d", &m, &n);
    // if(m < 1000000) {
    //     littleSize(m, n);
    //     return 0;
    // }
    // littleSize(m, n);
    char op; 
    int key;

    // int P[n], post=0;

    CuckooHash ht;
    ht.length = 1.3*m;
    initHashTable(&ht, ht.length);
    // show(&ht);

    for (int i = 0; i < n; i++) {
        // scanf(" %c%d", &op, &key);
        getchar();
        op = getchar();
        // gets(key);
        // key = (int) key;
        scanf("%d", &key);

        // if (op == 'L') {
        //     // P[post++] = lookUpKey(&ht, key);
        //     printf("%d\n", lookUpKey(&ht, key));
        // }
        // else if (op == 'I') {
        //     insertKey(&ht, key);
        //     // show(&ht);
        // }
        // else if (op == 'D') {
        //     deleteKey(&ht, key);
        //     // show(&ht);
        // }

        switch (op) {
        case 'L':
            printf("%d\n", lookUpKey(&ht, key));
            break;
        case 'I':
            insertKey(&ht, key);
            break;
        case 'D':
            deleteKey(&ht, key);
            break;
        default:
            break;
        }
    }
    return 0;
}