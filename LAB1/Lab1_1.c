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

// int compare(int i, int j, struct Student std[]){
//     if(std[i].score != std[j].score){
//         return (std[j].score - std[i].score);
//     }
//     else if(std[i].score1 != std[j].score1){
//         return (std[j].score1 - std[i].score1);
//     }
//     else if(strcmp(std[i].name, std[j].name) != 0){
//         return (strcmp(std[i].name, std[j].name));
//     }
//     else {
//         return (std[i].id - std[j].id);
//     }
// }

int partition(int p, int r, struct Student std[]){
    int k = (p + r)/2, i = p - 1;
    swap(k, r, std);
    for(int j = p; j < r; j++){
        if(compare(j, r, std)){
            i++;
            swap(i, j, std);
        }
    }
    swap(i+1, r, std);
    return i+1;
}

void quickSort(int p, int r, struct Student std[]){
    if(p < r){
        int q = partition(p, r, std);
        quickSort(p, q-1, std);
        quickSort(q+1, r, std);
    }
    return;
}


// void quickSort(int l, int r, struct Student std[]){
//     if(l >= r) return;
//     int k = (l + r)/2;
//     int i = l, j = r;
//     while (i <= j){
//         while(compare(i, k, std) > 0) i++;
//         while(compare(j, k, std) < 0) j--;
//         if(i <= j){
//             swap(i, j, std);
//             i++;
//             j--;
//         } 
//     }
//     quickSort(l, j, std);
//     quickSort(i, r, std);
// }


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
