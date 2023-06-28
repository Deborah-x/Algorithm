#include<stdio.h>
#include<string.h>

struct Student
{
    int id;
    char name[20];
    int score1, score2, score;
};

void swap(int i, int j, struct Student std[]){
    struct Student temp;
    temp = std[i]; std[i] = std[j]; std[j] = temp;
    return;
}

void radixSort_score(struct Student std[], int n) {
    // 计算最大值，确定位数
    int maxVal = 0;
    for (int i = 0; i < n; i++) {
        if (std[i].score > maxVal) {
            maxVal = std[i].score;
        }
    }

    // 对每一位进行计数排序
    for (int exp = 1; maxVal/exp > 0; exp *= 10) {
        // 初始化计数器
        int count[10] = {0};
        struct Student output[n];

        // 计算计数器
        for (int i = 0; i < n; i++) {
            int digit = (std[i].score / exp) % 10;
            count[digit]++;
        }

        // 将计数器转换为位置信息
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // 从前往后遍历原始数组，根据计数器将元素放到正确的位置上
        for (int i = 0; i < n; i++) {
            int digit = (std[i].score / exp) % 10;
            output[n - count[digit]] = std[i];
            count[digit]--;
        }

        // 将输出数组复制回原数组
        for (int i = 0; i < n; i++) {
            std[i] = output[i];
        }
    }
}

void radixSort_score1(struct Student std[], int n) {
    // 计算最大值，确定位数
    int maxVal = 0;
    for (int i = 0; i < n; i++) {
        if (std[i].score1 > maxVal) {
            maxVal = std[i].score1;
        }
    }

    // 对每一位进行计数排序
    for (int exp = 1; maxVal/exp > 0; exp *= 10) {
        // 初始化计数器
        int count[10] = {0};
        struct Student output[n];

        // 计算计数器
        for (int i = 0; i < n; i++) {
            int digit = (std[i].score1 / exp) % 10;
            count[digit]++;
        }

        // 将计数器转换为位置信息
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // 从前往后遍历原始数组，根据计数器将元素放到正确的位置上
        for (int i = 0; i < n; i++) {
            int digit = (std[i].score1 / exp) % 10;
            output[n - count[digit]] = std[i];
            count[digit]--;
        }

        // 将输出数组复制回原数组
        for (int i = 0; i < n; i++) {
            std[i] = output[i];
        }
    }
}

void insertSort_name(struct Student std[], int n) {
    int i, j;
    struct Student key;
    for (i = 1; i < n; i++) {
        key = std[i];
        j = i - 1;
        while ((j >= 0) && strcmp(std[j].name, key.name) > 0) {
            std[j+1] = std[j];
            j--;
        }
        std[j+1] = key;
    }
}

void radixSort_id(struct Student std[], int n) {
    // 计算最大值，确定位数
    int maxVal = 0;
    for (int i = 0; i < n; i++) {
        if (std[i].id > maxVal) {
            maxVal = std[i].id;
        }
    }

    // 对每一位进行计数排序
    for (int exp = 1; maxVal/exp > 0; exp *= 10) {
        // 初始化计数器
        int count[10] = {0};
        struct Student output[n];

        // 计算计数器
        for (int i = 0; i < n; i++) {
            int digit = (std[i].id / exp) % 10;
            count[digit]++;
        }

        // 将计数器转换为位置信息
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // 从后往前遍历原始数组，根据计数器将元素放到正确的位置上
        for (int i = n - 1; i >= 0; i--) {
            int digit = (std[i].id / exp) % 10;
            output[count[digit] - 1] = std[i];
            count[digit]--;
        }

        // 将输出数组复制回原数组
        for (int i = 0; i < n; i++) {
            std[i] = output[i];
        }
    }
}

int main() {
    int num;
    scanf("%d", &num);
    struct Student std[num];
    for (int i = 0; i < num; i++) {
        scanf("%d%s%d%d", &std[i].id, std[i].name, &std[i].score1, &std[i].score2);
        std[i].score = std[i].score1 + std[i].score2;
    }
    radixSort_id(std, num);
    insertSort_name(std, num);
    radixSort_score1(std, num);
    radixSort_score(std, num);
    for (int i = 0; i < num; i++) {
        // printf("%06d %s %d %d\n", std[i].id, std[i].name, std[i].score1, std[i].score2);
        printf("%06d\n", std[i].id);
    }
    return 0;
}
