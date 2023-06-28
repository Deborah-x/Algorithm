#include<stdio.h>
#include<math.h>

int flag = 0;

struct POS{
    double x, y;
}points[1000000];

double distance(struct POS p1, struct POS p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

void swap(int i, int j){
    struct POS temp;
    temp = points[i]; points[i] = points[j]; points[j] = temp;
    return;
}

int compare(int i, int j){
    if(flag % 2 == 0){
        if(points[i].x <= points[j].x) return 1;
        return 0;
    }
    else{
        if(points[i].y <= points[j].y) return 1;
        return 0;
    }
    
}

int randomPartition(int p, int r){
    int k = (p + r)/2, i = p - 1;
    swap(k, r);
    for(int j = p; j < r; j++){
        if(compare(j, r)){
            i++;
            swap(i, j);
        }
    }
    swap(i+1, r);
    return i+1;
}

// int randomPartition(int p, int r){
//     struct POS ins[r-p+1];
//     int a = 0, b = r-p;
//     int mid = (p+r)/2;
//     for(int i = 0; i < r-p+1; i++){
//         if(i == mid) continue;
//         if(compare(i, mid)){
//             ins[a++] = points[i];
//         }
//         else ins[b--] = points[i];
//     }
//     ins[a] = points[mid];
//     for(int i = 0; i < r-p+1; i++){
//         points[p+i] = ins[i];
//     }
//     return mid;
// }

int randomSelect(int p, int r, int i){
    if(p == r) return p;
    int q = randomPartition(p, r);
    int k = q - p + 1;
    if(i == k) return q;
    else if (i < k){
        return randomSelect(p, q-1, i);
    }
    else return randomSelect(q+1, r, i-k);
}

double nearestNeighborSearch(int p, int r){
    if (p == r) return INFINITY;
    if (p + 1 == r) return distance(points[p], points[r]);
    
    int mid = randomSelect(p, r, (r-p+1)/2);
    double d = fmin(nearestNeighborSearch(p, mid), nearestNeighborSearch(mid+1, r));
    
    struct POS strip[r-p+1];
    int j = 0;
    for (int i = p; i <= r; i++) {
        if (fabs(points[i].x - points[mid].x) < d) {
            strip[j++] = points[i];
        }
    }
    
    for (int i = 0; i < j; i++) {
        for (int k = i+1; k < j && (strip[k].y - strip[i].y) < d; k++) {
            double dist = distance(strip[i], strip[k]);
            if (dist > 0 && dist < d) {
                d = dist;
            }
        }
    }
    
    return d;
}

int main(){
    int num;
    scanf("%d", &num);
    
    for(int i = 0; i < num; i++){
        scanf("%lf", &points[i].x);
        scanf("%lf", &points[i].y);
    }
    printf("%f\n", nearestNeighborSearch( 0, num-1));
    return 0;
}