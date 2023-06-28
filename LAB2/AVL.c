#include<stdio.h>
#include<stdlib.h>
  
//结点设计 
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} BTNode;
  
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}
  
int max(int a, int b) {
    return (a > b) ? a : b;
}
  
BTNode* newNode(int key) {
    struct Node* node = (BTNode*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}
  
//ll型调整 
BTNode* ll_rotate(BTNode* y) {
    BTNode *x = y->left;
    y->left = x->right;
    x->right = y;
  
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
  
    return x;
}
  
//rr型调整 
BTNode* rr_rotate(BTNode* y) {
    BTNode *x = y->right;
    y->right = x->left;
    x->left = y;
  
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
  
    return x;
}
  
//判断平衡
int getBalance(BTNode* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
  
//插入结点&数据
BTNode* insert(BTNode* node, int key) {
    if (node == NULL)
        return newNode(key);
  
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;
  
    node->height = 1 + max(height(node->left), height(node->right));
  
    int balance = getBalance(node);
  
    if (balance > 1 && key < node->left->key) //LL型
        return ll_rotate(node);
  
    if (balance < -1 && key > node->right->key)     //RR型
        return rr_rotate(node);
  
    if (balance > 1 && key > node->left->key) {   //LR型
        node->left = rr_rotate(node->left);
        return ll_rotate(node);
    }
  
    if (balance < -1 && key < node->right->key) {   //RL型
        node->right = ll_rotate(node->right);
        return rr_rotate(node);
    }
  
    return node;
}
  
//遍历
void inOrder(struct Node *root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}
  
int main() {
    BTNode *root = NULL;
  
    root = insert(root, 2);
    root = insert(root, 1);
    root = insert(root, 0);
    root = insert(root, 3);
    root = insert(root, 4);
    root = insert(root, 4);
    root = insert(root, 5);
    root = insert(root, 6);
    root = insert(root, 9);
    root = insert(root, 8);
    root = insert(root, 7);
  
    printf("中序遍历：");
    inOrder(root);
    return 0;
}