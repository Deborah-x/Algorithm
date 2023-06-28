#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#define MAX 450001
using namespace std;

int parent[5001];
int father[5001];
int visit[MAX];
int n, m;
int i, j;
 
struct edge{
	int u, v, w; // 边的顶点，权值
}edges[MAX];
 
// 初始化并查集
void UFset(){
	for(i = 1; i <= n; i++) 
		parent[i] = -1;
}

// 标记某条边是否已加入最小生成树中
void init(){
	for(i = 0; i <= m; i++)
		visit[i] = 0;
}
 
// 查找 i 的根
int find(int i){
	int temp;
	// 查找位置
	for(temp = i; parent[temp] >= 0; temp = parent[temp]);
	// 压缩路径
	while(temp != i){
		int t = parent[i];
		parent[i] = temp;
		i = t;
	}
	return temp;
}

// 合并两个元素a,b
void merge(int a,int b){
	int r1 = find(a);
	int r2 = find(b);
	int tmp = parent[r1] + parent[r2]; // 两个集合节点数的和
	if(parent[r1] > parent[r2]){
		parent[r1] = r2;
		parent[r2] = tmp;
	}else{
		parent[r2] = r1;
		parent[r1] = tmp;
	}
}

// void subMinTree(int minWeight) {
// 	int Weight = 0;
// 	int delta = 268435455;
// 	int u, v;
// 	for(int i = 0; i < m; i++) {
// 		if(visit[i] == 1) {
// 			for(int j = 0; j < m; j++) {
// 				if(visit[j] == 0) {
// 					printf("i=%d j=%d\n", i, j);
// 					u = edges[j].u;
// 					v = edges[j].v;
// 					if(find(u) != find(v)) {
// 						delta = min(delta, edges[j].w - edges[i].w);
// 					}
// 				}
				
// 			}
// 		}
// 	}
// 	Weight = minWeight + delta;
// 	printf("sub Min Tree is %d\n", Weight);
// }

void subMinTree() {
	int sumWeight;
	int Weight = 268435455;
	int u,v;
	for(int j = 0; j < m; j++) {
		if(visit[j] == 1) {
			sumWeight = 0;
			UFset();
			for(int i = 0; i < m; i++)
			{
				// printf("i:%d j:%d\n", i, j);
				if(i == j) 
					continue;

				u = edges[i].u;
				v = edges[i].v;
		
				if(find(u) != find(v)){
					// printf("加入边：%d %d,权值： %d\n", u,v,edges[i].w);
					sumWeight += edges[i].w;
					merge(u, v);
				}
			}
			Weight = min(Weight, sumWeight);
		}
	}

	// printf("sub weight of MST is %d \n", Weight);
	printf("%d\n", Weight);
}
 
void kruskal(){
	int sumWeight = 0;
	int num = 0;
	int u,v;
	UFset();
	init();

	for(int i = 0; i < m; i++)
	{
		u = edges[i].u;
		v = edges[i].v;
 
		if(find(u) != find(v)){ // u 和 v 不在一个集合
			// printf("加入边：%d %d,权值： %d\n", u,v,edges[i].w);
			sumWeight += edges[i].w;
			num ++;
			merge(u, v); // 把这两个边加入一个集合。
			visit[i] = 1;
		}
	}
	// for(i = 0; i < m; i++) 
	// 	printf("%d ", visit[i]);
	// printf("\n");
	// printf("weight of MST is %d \n", sumWeight);
	printf("%d\n", sumWeight);
}
 
// 比较函数，用户排序
int cmp(const void * a, const void * b){
	edge * e1 = (edge *)a;
	edge * e2 = (edge *)b;
	return e1->w - e2->w;
}
 
int main() {
	memset(father, -1, sizeof(father));

	scanf("%d %d", &n, &m);
	for(i = 0; i < m; i++){
		scanf("%d %d %d", &edges[i].u,  &edges[i].v,  &edges[i].w);

	}
	qsort(edges, m, sizeof(edge), cmp);
 
	kruskal();
	subMinTree();
	return 0;
}