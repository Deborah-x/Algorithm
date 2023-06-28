#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LENGTH  	200000
#define GOLD_RATE   0.6180339887

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
 
// 哈希查找
struct Hash{
    int id;
    int score;
    struct Hash *next;
};

int sum;	// 统计目前树中节点总数


Node *Tree_Minimum(Node *T) {
	while(T->left != T_NIL)
		T=T->left;
	return T;
}

//中序遍历树T,输出
void Inorder_Tree_Walk(Node *T) {
	if ( T != T_NIL) {
		Inorder_Tree_Walk(T->left);   //递归其左孩子
		printf("id:%d\t score:%d\n", T->id, T->score);
		if (T->color == 0) {
			printf("-R  ");     //输出结点的颜色
		}
        else {
			printf("-B  ");
		}
		Inorder_Tree_Walk(T->right);  //递归其右孩子
	}
}

void Pre_Tree_Walk(Node *T) {
	if ( T != T_NIL) {     
		printf("id:%d\t score:%d\t size:%d\n", T->id, T->score, T->size);
		Pre_Tree_Walk(T->left);   //递归其左孩子      
		Pre_Tree_Walk(T->right);  //递归其右孩子
		
	}
}
 
void Left_Rotate(Node **T,Node *x) {
  	Node *y=x->right;
  
   	x->right =y->left;
   	if (y->left != T_NIL)
       	y->left->p=x;
   	y->p=x->p;
   	if(x->p==T_NIL)
    	*T=y;
   	else if (x == x->p->left)
      	x->p->left = y;
   	else
	  	x->p->right = y;
   	y->left = x;
   	x->p=y;

	while(x != T_NIL){
	    x->size = x->left->size + x->right->size + 1;
	    x = x->p;
	}
}

void Right_Rotate(Node **T,Node *y) {
	Node *x=y->left;
 
	y->left =x->right;
	if (x->right != T_NIL)
		x->right->p=y;
	x->p=y->p;
	if(y->p==T_NIL)
		*T=x;
	else if (y == y->p->left)
		y->p->left = x;
	else  
		y->p->right = x;
	x->right = y;
	y->p=x;

	while(y != T_NIL){
	    y->size = y->left->size + y->right->size + 1;
	    y = y->p;
	}
}

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

Node *RB_Insert_Fixup(Node *T,Node *z) {
	// printf("IN\n");
	Node *y=NULL;
   	while( z->p->color == red)       //违反了性质4，迭代进行修正
   	{
      	if (z->p == z->p->p->left)
      	{
          	y = z->p->p->right;
		  	if ( y->color == red)    // case 1 叔结点为红色
		  	{
             	z->p->color = black;    //父节点涂黑
			 	y->color = black;       //叔结点涂黑
			 	z->p->p->color = red;   //祖结点涂红
			 	z = z->p->p;            //向上迭代，更新z的位置
		  	}
		  	else if ( z == z->p->right) //case 2 叔结点为黑色且z为双亲的右孩子
		  	{
             	z = z->p;
			 	Left_Rotate(&T,z);
			 	z->p->color = black;    //case2 已转为 case3 继续处理
			 	z->p->p->color = red;
			 	Right_Rotate(&T,z->p->p);// while循环终止
		  	}
		  	else                      // case 3 叔结点为黑色且z为双亲的左孩子 
		  	{
             	z->p->color = black;
			 	z->p->p->color = red;
			 	Right_Rotate(&T,z->p->p);//   while循环终止 
		  	}
      	}
	  	else                      //对称处理
	  	{
	
		  	y = z->p->p->left;
		  	if ( y->color == red)    // case 1 叔结点为红色
		  	{
			  	z->p->color = black;
			  	y->color = black;
			  	z->p->p->color = red;
			  	z = z->p->p;
		  	}
		  
		  	else if ( z == z->p->left) //case 2 叔结点为黑色且z为双亲的右孩子
		  	{
			  	z = z->p;
			  	Right_Rotate(&T,z);
			  	z->p->color = black;
			  	z->p->p->color = red;
			  	Left_Rotate(&T,z->p->p);// 
		  	}
		  	else                      // case 3
		  	{
			  	z->p->color = black;
			  	z->p->p->color = red;
			  	Left_Rotate(&T,z->p->p); 
		  	}
	  	}  
   	}
	// printf("OUT\n");
   	T->color = black;          //保证不会违反性质2，对根节点涂黑
   	return T;
}

Node *RB_Insert(Node *Root,Node *z)   //红黑树插入,返回树的根
{
	Node *y=T_NIL;
	Node *x=Root;
	while( x != T_NIL)                 //找到结点z要插入的位置    
	{
		y=x;
		if (Compare(z, x) < 0)
			x = x->left;
		else if(Compare(z, x) > 0)
			x = x->right;
		else 
			return Root;
	}
	z->p = y;
	if ( y == T_NIL)    //插入第一个结点作为根节点的情况  
		Root = z;
	else if (Compare(z, y) < 0)
		y->left = z;
	else 	
		y->right = z;
	
	while(y != T_NIL){
	    y->size = y->left->size + y->right->size + 1;
	    y = y->p;
	}

	Root = RB_Insert_Fixup(Root,z);    //插入完毕后，对红黑树的颜色进行修正
	
	return Root; 
}

Node *Establish()   //建立红黑树
{
   Node * T,*node;
   int i=0;
   node=NULL;
   T_NIL=(Node *)malloc(sizeof(Node));  //建立T_NIL结点
   T_NIL->p=NULL;
   T_NIL->left=NULL;
   T_NIL->right=NULL;
   T_NIL->id=-1;
   T_NIL->size = 0;
   T_NIL->color=black;
   T=T_NIL;
   return T;
}
 
void RB_Transplant(Node **T,Node *u,Node *v)  //结点替代函数
{
  	if (u->p == T_NIL)
      	*T = v;
  	else if (u == u->p->left)
     	u->p->left = v;
  	else
	 	u->p->right = v;
  	v->p = u->p;               //此处赋值无条件，v如果是T_NIL也要进行赋值
}

Node *RB_Delete_Fixup(Node *T,Node *x) {
	Node *w=NULL;
  	while( x != T && x->color == black)      //循环迭代处理
  	{
	  	if ( x == x->p->left ) {
        	w = x->p->right;
		if (w->color == red)             // case 1 ------> case 2 , case 3 ,case 4 
		{
			w->color = black;
			x->p->color =red;
			Left_Rotate(&T,x->p);
			w = x->p->right;
		}
		if ( w->left->color == black && w->right->color == black ) //case 2 ------>go on / stop
		{
            w->color = red;
			x = x->p;
		}
		else if ( w->right->color == black)   // case 3 ---->case 4---->stop
		{
           w->left->color = black;
		   w->color =red ;
		   Right_Rotate(&T,w);
 
		   w = x->p->right ;                   //转成case 4处理 
		   w->color = x->p->color;
		   x->p->color = black;
		   w->right->color = black;
		   Left_Rotate(&T,x->p);
		   x = T;
		}
		else                               // case 4 ------------------->stop
		{
			w->color = x->p->color;
			x->p->color = black;
			w->right->color = black;
			Left_Rotate(&T,x->p);
		    x = T;
		}
	  	}
	  	else
	  	{
		  	w = x->p->left;
		  	if (w->color == red)                // case 1 ------> case 2 , case 3 ,case 4 
		  	{
			  	w->color = black;
			  	x->p->color =red;
			  	Right_Rotate(&T,x->p);
			  	w = x->p->left;
		  	}
		  	if ( w->right->color == black && w->left->color == black ) //case 2 ------>go on/stop
		  	{
			  	w->color = red;
			  	x = x->p;
		  	}
		  	else if ( w->left->color == black)      // case 3 -----> case 4----->stop
		  	{
			  	w->right->color = black;
			  	w->color =red ;
			  	Left_Rotate(&T,w);
			  
			  	w = x->p->left ;                    //转成case 4处理
			  	w->color = x->p->color;
			  	x->p->color = black;
			  	w->left->color = black;
			  	Right_Rotate(&T,x->p);
			  	x = T;
		  	}
		  	else                                  // case 4 -------------->stop
		  	{
			  	w->color = x->p->color;
			  	x->p->color = black;
			  	w->left->color = black;
			  	Right_Rotate(&T,x->p);
			  	x = T;
			}
	  	}
  	}
 
    x->color = black;                       //可能由case2退出，那把x涂黑即可，见分析！也可能有case4退出，把根节点涂黑
    return T;
}

Node *RB_Delete(Node *T ,Node *z) {
	if(z == NULL) return T;
	Node *x =NULL;
	Node *y =z;
	enum colors y_original_color = y->color;   	//记录下删除前z的颜色
    if ( z->left == T_NIL)                     	//左子树不存在的情况  
    {
       	x = z->right;
	   	RB_Transplant(&T,z,z->right);
    }
	else if ( z->right == T_NIL)              	//右子树不存在
	{
	   	x = z->left;
	   	RB_Transplant(&T,z,z->left);
	}
	else                                     	//左右都存在的情况
	{
       	y = Tree_Minimum(z->right);            	//找到后继y
	   	y_original_color = y->color;           	//记录下y转移前的颜色
	   	x = y->right;
	   	if ( y->p == z)                       	//如果y是z的子结点
	   	{
         	x->p = y;
	   	}
	   	else
	   	{
		   	RB_Transplant(&T,y,y->right);    	//如果y不是z的子结点，用y的右子树代替y的位置
		   	y->right = z->right;
		   	y->right->p = y;
	   	}
	   
	   	RB_Transplant(&T,z,y);           		//y替代z的位置 ，不论y是不是T_NIL   
	   	y->left = z->left;
	   	y->left->p = y;
	   	y->color = z->color;             		//把y的颜色改成z的颜色
	}

	Node *w = x->p;
	while(w != T_NIL){
	    w->size = w->left->size + w->right->size + 1;
	    w = w->p;
	}
 
	if ( y_original_color == black)   			//判断y的颜色，若为黑色，需要修复
	   	T = RB_Delete_Fixup(T,x);
	return T;
}
 
Node *Tree_Search(Node *T ,int key1, int key2)  //寻找数k是否在树中，且返回数k的地址
{	
    while(T !=T_NIL && (T->id != key1 || T->score != key2)) {
		if ( key2 < T->score)
			T=T->left;
		else if (key2 > T->score)
			T=T->right;
		else {
			if(key1 < T->id)
				T=T->left;
			else
				T=T->right;
		}
	}
    
	if ( T == T_NIL) {     
		return NULL;
	}
	else {
		return T;
	}
	
}

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

// 用Hash实现查找，用链表解决冲突
int hash(int length, int key) {
    double s = key*GOLD_RATE - floor(key*GOLD_RATE);
	return floor(length*s);
}

void initHashTable(struct Hash ht[], int length) {
    for(int i = 0; i < length; i++) {
        ht[i].id = -1; 
        ht[i].score = 0;
        ht[i].next = NULL;
    }
}

void showHashTable(struct Hash ht[], int length) {
    for(int i = 0; i < length; i++) {
        printf("%d\t%d\n", ht[i].id, ht[i].score);
    }
    printf("\n");
}

int lookUpKey(struct Hash ht[], int length, int key) {
    int index = hash(length, key);
    if(ht[index].id == key) return ht[index].score;
    struct Hash *ptr = &ht[index];
    while(ptr->next != NULL) {
        ptr = ptr->next;
        if(ptr->id == key) {
            return ptr->score;
        }
    }
    return 0;
}

void insertHashKey(struct Hash ht[], int length, int key, int value) {
    int index = hash(length, key);
    if(ht[index].id == -1) {
        ht[index].id = key;
        ht[index].score = value;
        // ht[index].next = NULL;
        return;
    }
    struct Hash *ptr = (struct Hash*)malloc(sizeof(struct Hash));
    ptr->id = key;
    ptr->score = value;
    ptr->next = ht[index].next;
    ht[index].next = ptr;
    // Hash *ptr1, *ptr2 = ht[index];
    // if(value < ht[index].score) {
    //     ptr1->id = ht[index].id;
    //     ptr1->score = ht[index].score;
    //     ht[index].id = key;
    //     ht[index].score = value;
    // }
    // while(ptr2->next != NULL) {
    //     ptr2 = ptr2->next;
    //     // 一个减小查找和删除时间的方法是在插入key时维持链表升序
    // }
    return;
}

void deleteHashKey(struct Hash ht[], int length, int key, int value) {
    int index = hash(length, key);
    if((ht[index].id == key) && (ht[index].score == value)) {
        if(ht[index].next == NULL) {
            ht[index].id = -1;
            return;
        }
        struct Hash *ptr = ht[index].next;
        ht[index].id = ptr->id;
        ht[index].score = ptr->score;
        ht[index].next = ptr->next;
        free(ptr);
        return;
    }
    struct Hash *ptr = &ht[index];
    while(ptr->next != NULL) {
        if(ptr->next->id == key && ptr->next->score == value) {
            struct Hash *del = ptr->next;
            ptr->next = del->next;
            free(del);
            return;
        }
        ptr = ptr->next;
    }
    return;
}

void showNode(struct Node *node) {
    printf("id:%d\t score:%d\t color:%d\t\n", node->id, node->score, node->color);
}

void main()
{
    long long n;  // 输入规模 n
    scanf("%lld", &n);

	struct Hash ht[LENGTH];
	initHashTable(ht, LENGTH);
	// showHashTable(ht, LENGTH);

    Node *root=Establish();
    char op;
    long long int key1, key2;
    for(int i = 0; i < n; i++) {
        getchar();
        op = getchar();
        // scanf("%d", &key1);
        switch(op) {
        case 'I':
			scanf("%d", &key1);
            scanf("%d", &key2);
			insertHashKey(ht, LENGTH, key1, key2);
            struct Node *z1 = (struct Node*)malloc(sizeof(struct Node));
            z1->id = key1;
            z1->score = key2;
			z1->p = T_NIL;
            z1->left = T_NIL;
            z1->right = T_NIL;
            z1->color = red;
            z1->size = 1;
            root = RB_Insert(root, z1);
            // Pre_Tree_Walk(root);
            // showHashTable(ht, LENGTH);
			sum++;
            // printf("%c\t%d\t%d\n", op, key1, key2);
            break;

        case 'D':
			scanf("%d", &key1);
            scanf("%d", &key2);
			deleteHashKey(ht, LENGTH, key1, key2);
            root = RB_Delete(root, Tree_Search(root, key1, key2));
            // Pre_Tree_Walk(root);
            // showHashTable(ht, LENGTH);
			sum--;
            // printf("%c\t%d\t%d\n", op, key1, key2);
            break;

        case 'L':
			scanf("%d", &key1);
            printf("%d\n", lookUpKey(ht, LENGTH, key1));
            // printf("%c\t%d\n", op, key1);
            break;

        case 'S':
			scanf("%d", &key1);
			// printf("key=%d\t sum=%d\n", key1, sum);
			if(key1 > sum) printf("-1\n");
			else
				printf("%d\n", RB_Select(root, key1));		// key1 <= sum, there must exist the kth largest score
            // printf("%c\t%d\n", op, key1);
			break;

        case 'M':
			scanf("%d", &key1);
			printf("%d\n", RB_Minimum(root, key1));
            // printf("%c\t%d\n", op, key1);
            break;

        case 'C':
			scanf("%lld", &key1);
            scanf("%lld", &key2);
            printf("%d\n", RB_Count(root, key1, key2));
			// printf("%d\n", -1);
            // printf("%c\t%d\t%d\n", op, key1, key2);
            break;

        default:
            break;
        }
        
    }

}