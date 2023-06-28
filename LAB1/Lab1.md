# Lab 1-PB20000178李笑
## Q1: 快速排序

根据题目描述的参与者的个人信息，我们可以创建如下的结构体

```C
struct Student
{
    int id;
    char name[20];
    int score1, score2, score;
};
```

在`main()`函数中我们初始化结构体数组`std[num]`，从键盘输入，排序，然后输出

```C
int main(){
    int num;
    scanf("%d", &num);
    struct Student std[num];
    for(int i = 0; i < num; i++){
        scanf("%d%s%d%d", &std[i].id, std[i].name, &std[i].score1, &std[i].score2);
        std[i].score = std[i].score1 + std[i].score2;
    }
    quickSort(0, num-1, std);
    for(int i = 0; i < num; i++){
        printf("%06d\n", std[i].id);
    }
    return 0;
}
```

此题所用快速排序算法参照PPT所写，因此不再赘述。唯一值得展示的是此题用到的比较函数`compare()`，为了满足题目要求的排序顺序，我们依次对笔试总分、笔试一成绩、姓名、准考证号进行比较

```C
int compare(int i, int j, struct Student std[]){
    if(std[i].score != std[j].score){
        return (std[i].score > std[j].score ? 1 : 0);
    }
    else if(std[i].score1 != std[j].score1){
        return (std[i].score1 > std[j].score1 ? 1 : 0);
    }
    else if(strcmp(std[i].name, std[j].name) != 0){
        return (strcmp(std[i].name, std[j].name) < 0 ? 1 : 0);
    }
    else {
        return (std[i].id < std[j].id ? 1 : 0);
    }
}
```


## Q2: 基数排序

这题数据读取、输出与Q1完全相同，只是将排序方法从快速排序算法改为基数排序算法。

根据PPT提供的基数排序算法我们可以写出分别对score、score1、id的排序

```C
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
    ……
}

void radixSort_id(struct Student std[], int n) {
    ……
}
```
值得注意的是，PPT上的是升序排列，我们这里对分数降序需要在遍历放置元素的时候进行一定的修改

因为name是长度不等的字符串，基数排序对其不适用，但是我们必须选择一个稳定的排序算法，保证相对有序性。这里我们使用的是插入排序

```C
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
```

## Q3: FFT实现卷积计算
方法与Q4相同，因为当输入数组的长度为num时，如果我们将数组0填充到`n>2*num`，卷积后得到的数组将会如有性质——`c[i] + c[i+num]`等于原来要求的数组`c[i]`，因此我们只需要在Q4的基础上修改几行代码即可。
```C
for(int i = 0; i < num; i++) {
        c[i] = c[i] + c[i+(int)(num)];
}
for(int i = 0; i < num; i++) {
    printf("%d\n", c[i]);
}
```


## Q4: FFT 实现多项式乘法
根据算法导论提供的公式推导以及FFT伪码，我们可以写出如下的快速傅里叶变换和快速傅里叶反变换算法的代码

```C
void FFT(int n, int a[], double complex Y[]) {  // n is power of 2
    if(n == 1) {
        Y[0] = a[0];
        return;
    }
    int odd[n/2], even[n/2];
    for(int i = 0; i < n; i++) {
        if(i%2 == 0) even[i/2] = a[i];
        else odd[i/2] = a[i];
    }
    double complex S0[n], S1[n];
    FFT(n/2, even, S0);
    FFT(n/2, odd, S1);
    double complex z = (2*PI/n)*I;
    double complex w = cexp(z);
    double complex wk = 1;
    for(int k = 0; k < n/2; k++) {
        Y[k] = S0[k] + S1[k] * wk;
        Y[k+n/2] = S0[k] - S1[k] * wk;
        wk *= w;
    }
    return;
}

void IFFT(int n, double complex Y[], double complex y[]) {
    if(n == 1) {
        y[0] = Y[0];
        return;
    }
    double complex Y0[n/2], Y1[n/2];
    for(int i = 0; i < n; i++) {
        if(i%2 == 0) Y0[i/2] = Y[i];
        else Y1[i/2] = Y[i];
    }
    double complex y0[n], y1[n];
    IFFT(n/2, Y0, y0);
    IFFT(n/2, Y1, y1);
    double complex z = (2*PI/n)*I;
    double complex w = cexp(-z);
    double complex wk = 1;
    for(int k = 0; k < n/2; k++) {
        y[k] = y0[k] + y1[k] * wk;
        y[k+n/2] = y0[k] - y1[k] * wk;
        wk *= w;
    }
    
    return;
}
```

为了保证能使用FFT，对于一般情况（即输入数组长度不为2的幂指数次时），我们需要将数组用0填充到长度为2的幂指数次

函数`find`用来找到大于等于输入长度n的最小的2幂指数

```C
float find(float n) {
    if(n <= 1) {
        n = 1;
        return n;
    }
    n = find(n/2);
    return n * 2;
}
```

两个次数界为n的多项式相乘得到的多项式次数界为2n-1，但为了保证长度为2的指数次，我们选择将数组的长度构造为2n。接下来我们只需要将输入数组通过FFT变换到点值域，然后将对应点值相乘，结果通过IFFT变换回系数形式，最后因为我们多填充了0，所以需要将最高次连续为0的系数舍去。即，
```C
int tail = 2*n-1;
while(c[tail] == 0) {
    tail--;
}
for(int i = 0; i <= tail; i++) {
    printf("%d\n", c[i]);
}
```
