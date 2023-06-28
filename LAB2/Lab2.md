# Lab 2-PB20000178李笑
## Q1: 最近点对

这题要求查找所给数据点中最近点对的距离，且时间复杂度要求为O(nlogn)。因此我们使用如下结构体来存储每个数据点的横纵坐标，并在结构体定义的同时开1000000长度的数组。在开辟数组的时候总结发现一个规律——在主函数中开辟数组长度上限较小，且开辟同样长的数组时间要大于全局数组。

```C
struct POS{
    double x, y;
}points[1000000];
```

算法主要部分我们采用PPT上所述的平面最近点对算法，不断将查找范围二分、处理、合并。算法具体实现如下。其中`randomSelect()`函数用来选择中点用于进行二分，具体实现方法参照算法导论第9章，`strip[]`数组用于存储距离分割平面距离小于d的数据点，因为这些数据点在合并的时候可能会产生比左右两边分别讨论距离更近的点对。函数最后返回值是所有输入数据点最近点对的距离。

```C
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
```


## Q2: Cuckoo Hash

根据题目要求，我们需要实现名为`Cuckoo Hash`的哈希数据结构，并完成`lookUp(key)`、`insertKey(key)`、`delete(key)`三个功能。
首先，我们根据输入的m开辟两个长为1.3m的Bucket数组，且数组元素初始化为-1用来标明该位置为空。

`lookUpKey(key)`实现只需在两个Bucket中分别查看对应hash位置是否存在我们要找的值。若存在，返回1；否则，返回-1；

```C
int lookUpKey(CuckooHash *ht, int key) {
    if(ht->Bucket1[hash1(ht, key)] == key) {
        return 1;
    }
    else if(ht->Bucket2[hash2(ht, key)] == key) {
        return 1;
    }
    return 0;
}
```

`deleteKey(key)`实现也只需在两个Bucket中分别查看对应的hash位置是否存在我们要找的值。若存在，将对应位置置-1，标明此位置为空；否则，不处理，返回。

```C
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
}
```

`insetKey(key)`的实现略复杂。首先检查Bucket1对应位置是否为空，若是，直接插入；否则将对应元素踢出（即要插入元素取代原位置元素），原位置元素作为新的key在Bucket2中查找位置插入，若又不为空，继续置换踢出。此过程最多进行MAX_ITER次，若循环超过MAX_ITER次，则表明插入失败。鉴于本人能力有限没能找到完全不会发生冲突的hash函数，于是只能使用一点小trick——多开辟一个小数组用于存储插入失败的key。在开辟这个小数组后，查找和删除操作都需要检查该数组中是否存在对应key，过程简单在此不一一写名。添加数组后程序运行时间较之前增加，但能勉强通过测试。

```C
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
    }

    for(int i = 0; i < MAX_TS; i++) {
        if(ht->TS[i] == -1) {
            ht->TS[i] = key;
            return;
        }
    }

    return;
}
```

## Q3: 红黑树

第三题题目要求使用红黑树的数据结构实现如下几个功能：
1. `Insert(ID, Key)`：插入一个新元素。
2. `Delete(ID)`：删除一个元素。
3. `Lookup(ID)`：返回该元素的 score。
4. `Select(k)`：返回 score 第 k 大的元素 ID。
5. `Minimum(k)`：返回在 [k, +∞) 区间内，score 最小的 ID（如果有多个最小 score，返回ID 最小的）。
6. `Count(L, R)`：统计 score 在 [L, R] 区间内，有多少个元素

其中`insert()`、`delete()`都可以按照算法导论上伪码进行书写，并通过`rightRotate()`、`leftRotate()`、`insertFixUp()`、`deleteFixUp()`等函数来维护红黑树的基本性质。

红黑树节点的结构体定义如下，其中我们多定义了一个size变量用来存储某个节点子树大小（包括自己），所以插入的每个新节点的size都初始化为1。且在每次插入、删除、旋转操作的时候都要进行size的更新。

```C
enum colors{red,black};		// 枚举类型
// 红黑树的节点
typedef struct Node {
	int id;
  	int score;
	int size;				// 子树大小
   	struct Node *p;
   	struct Node *left;
   	struct Node *right;
   	enum colors color;
}Node;
Node *T_NIL=NULL;                 // 建立全部变量 T_NIL
```

为实现O(1)时间复杂度的查找，`lookUp()`通过哈希来实现，并通过链式法则来解决冲突问题。在每次插入、删除操作的同时通过`insertHashKey()`和`deleteHashKey()`更新哈希数组。

哈希数组的结构体定义如下，其余具体操作见代码Lab2_3.c。

```C
struct Hash{
    int id;
    int score;
    struct Hash *next;
};
```

下面我将详细讲解实现较为trick的三个函数：
1. `Select()`:

首先检查根节点右孩子的size大小。若size小于k-1，说明第k大的score肯定在左子树上，于是缩小搜索范围，对左子树重新应用select函数，不过这里需要注意的是左子树select传递的k值为 k - T->right->size - 1 ，意为去掉右子树和目前节点重新在左子树中搜索第 k - T->right->size - 1 大的score；若size大于k-1，说明第k大的score肯定在右子树上，于是缩小搜索范围，对右子树重新应用select函数；若不满足以上两个条件则说明当前节点的score就为第k大的，直接返回T->id即可。

```C
int RB_Select(Node *T, int k) {
	if(T->right->size < k-1) {
		return RB_Select(T->left, k - T->right->size - 1);
	}
	else if(T->right->size > k-1){
		return RB_Select(T->right, k);
	}
	else {
		return T->id;
	}
}
```

2. `Minimum()`
这个函数的如下实现有个前提就是插入时在score相同的情况下id要有序，因为测试数据中可能会存在最小的score有多个id的情况，我们要选择其中最小的id。在score相同id有序的情况下，我们找到在 [k, +∞) 范围内score最小且id最小的元素只需要简单沿树进行查找就好了。具体的compare和minimum函数如下所示：

```C
int Compare(Node *a, Node *b) {
	if(a->score < b->score) 
		return -1;
	if(a->score > b->score) 
		return 1;
	if(a->id < b->id) 
		return -1;
	else if(a->id > b->id) 
		return 1;
	else 
		return 0;
}

int RB_Minimum(Node *T ,int k) {
	if(T == T_NIL) 
		return -1;
	if(T->score < k) {
		return RB_Minimum(T->right, k);
	}
	else {
		int least_id = RB_Minimum(T->left, k);
		if(least_id == -1) 
			return T->id;
		else 
			return least_id;
	}
}
```


3. `Count()`

这个函数最开始我是用递归实现的，代码正确但是时间复杂度高，oj会报Time Limit Exceeded，后来在引入size变量后找到更快速的求法。

首先，通过移动目前节点指针curr将指针移动倒score在 [l, r] 范围内的节点。
然后count自增1，表明在[l, r] 范围内的节点数加一。新建一个指针pl指向curr的左孩子，如果左孩子的小于l，那么左孩子的左孩子肯定都小于l，直接将pl指向pl的右孩子继续统计；如果左孩子的score大于l，那么左孩子的右孩子必定大于l，于是count可以加上pl->right->size，免去了搜索pl右子树的时间，然后将pl指向pl的左孩子重复上述步骤。对于curr的右子树也是类似操作，最后函数会返回统计到的在区间 [l, r] 范围内的元素个数。

```C
// 递归实现 Time Limit Exceeded
// int RB_Count(Node *T, int l, int r) {
//     if(T == T_NIL) {
//         return 0;
//     }
//     if(T->score < l) {
//         return RB_Count(T->right, l ,r);
//     }
//     if(T->score > r) {
//         return RB_Count(T->left, l, r);
//     }
//     return RB_Count(T->left, l, r) + RB_Count(T->right, l, r) + 1;
// }

int RB_Count(Node *T, long long int l, long long int r) {
	int count = 0;
	Node *curr = T;
    while(curr != T_NIL) {
		if(curr->score < l) {
			curr = curr->right;
		}
		else if(curr->score > r) {
			curr = curr->left;
		}
		else {
			break;
		}
	}
	if(curr == T_NIL) return 0;
	count ++;
	Node *pl=curr->left;
	while(pl != T_NIL) {
		if(pl->score >= l) {
			count += 1 + pl->right->size;
			pl = pl->left;
		}
		else {
			pl = pl->right;
		}
	}
	Node *pr=curr->right;
	while(pr != T_NIL) {
		if(pr->score <= r) {
			count += 1 + pr->left->size;
			pr = pr->right;
		}
		else {
			pr = pr->left;
		}
	}
	return count;
}
```