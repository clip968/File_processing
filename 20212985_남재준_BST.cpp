// #include <bits/stdc++.h>
#include<iostream>
using namespace std;

//Node 구조체 정의
struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    int size;

    Node() : left(nullptr), right(nullptr), height(1), size(1){}
};

// 부모 Node 찾기
Node* searchParent(Node* T, int searchKey){
    Node* q = nullptr;    
    Node* p = T;
    while (p != nullptr) {
        // searchKey을 가진 노드의 존재 여부 검사
        if (searchKey == p->key) return q;
        q = p;
        // q는 p의 부모 노드를 지시
        if (searchKey < p->key) p = p->left;
        else p = p->right;
        }
    return nullptr;
}

// 재귀로 delete
void clear(Node* T){
    if(T == nullptr) return;
    // 재귀로 clear
    clear(T->left);
    clear(T->right);

    delete T;
}

// 최소 노드 찾기
Node* minNode(Node* T){
    if(T == nullptr){
        return nullptr;
    }
    
    if(T->left == nullptr){
        return T;
    }
    // 왼쪽 노드로 재귀 돌리면서 최소값 찾기
    else{
        return minNode(T->left);
    }
}

// 최대 노드 찾기
Node* maxNode(Node* T){
    if(T == nullptr){
        return nullptr;
    }

    if(T->right == nullptr){
        return T;
    }
    // 오른쪽 노드로 재귀 돌리면서 최대값 찾기
    else{
        return maxNode(T->right);
    }
}

// max 구현
int max(int a, int b){
    return (a > b) ? a : b;
}

// 높이 구하기
int height(Node* T) {
    // height가 nullptr일 시 0 반환
    return (T == nullptr) ? 0 : T->height;
}

// 노드 수 구하기
int size(Node* T) {
    // size가 nullptr일 시 0 반환
    return (T == nullptr) ? 0 : T->size;
}

//노드 찾기
Node* searchBST(Node* T, int searchKey) {
    if (T == nullptr) return nullptr;

    if (searchKey == T->key) return T;
    if (searchKey < T->key) return searchBST(T->left, searchKey);

    else return searchBST(T->right, searchKey);
}

// Node 생성
Node* getBSTNode() {
    Node* newNode = new Node();
    return newNode;
}

// 노드 넣기
void insertBST(Node*& T, int newKey) {
    Node* p = T;
    Node* q = nullptr;

    // find position to insert newKey while storing parent node on stack
    while (p != nullptr) {
        if (newKey == p->key) return; // newKey already exists in T

        q = p;

        if (newKey < p->key) p = p->left;
        else p = p->right;
    }

    // create new node
    Node* newNode = getBSTNode();
    newNode->key = newKey;
    
    // insert newNode as a child of q
    if (q == nullptr) T = newNode;
    else if (newKey < q->key) q->left = newNode;
    else q->right = newNode;

    // update height while popping parent node from stack
    while (q != nullptr) {
        int left_height = (q->left != nullptr) ? height(q->left) : 0;
        int right_height = (q->right != nullptr) ? height(q->right) : 0;
        q->height = 1 + max(left_height, right_height);
        q->size = 1 + size(q->left) + size(q->right);
        q = searchParent(T, q->key);
    }
}

// 노드 지우기
void eraseBST(Node*& T, int deleteKey) {
    Node* p = T;
    Node* q = nullptr;
   

    // find position of deleteKey while storing parent node on stack
    while (p != nullptr && deleteKey != p->key) { 
        q = p;
        

        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    if (p == nullptr) return;  // deleteKey was not found

    // case of degree 2
    if (p->left != nullptr && p->right != nullptr) {
        Node* trash = nullptr;
        int iflag;
        
        // 높이가 왼쪽이 더 클 경우
        if(height(p->left) > height(p->right)){
            trash = maxNode(p->left);
            iflag = 0;
        }
        // 높이가 오른쪽이 더 클 경우
        else if(height(p->left) < height(p->right)){
            trash = minNode(p->right);
            iflag = 1;
        }
        // 높이가 같을 경우
        else{
            //size가 왼쪽이 더 클 때
            if(size(p->left) >= size(p->right)){
                trash = maxNode(p->left);
                iflag = 0;
            }
            //size가 오른쪽이 더 클 때
            else{
                trash = minNode(p->right);
                iflag = 1;
            }
        }
        p->key = trash->key;
        // iflag == 0(left) 일 경우 왼쪽 노드로 재귀 시작
        if(iflag == 0) eraseBST(p->left, trash->key);
        else eraseBST(p->right, trash->key);
    }
    // now degree of p is 0 or 1
    else{

        if (p->left == nullptr && p->right == nullptr) { // case of degree 0
            if (q == nullptr) {T = nullptr;} // case of root
            else if (q->left == p) q->left = nullptr;
            else q->right = nullptr;
            
    }

        else if (p->left != nullptr) { // case of degree 1 with left child
            if (q == nullptr) T = p->left; // case of root
            else if (q->left == p) q->left = p->left;
            else q->right = p->left;
           
    }

        else { // case of degree 1 with right child
            if (q == nullptr) T = p->right; // case of root
            else if (q->left == p) q->left = p->right;
            else q->right = p->right;
            
    }

        delete p;

    // update height while popping parent node from stack
        while (q != nullptr) {
            int left_height = (q->left != nullptr) ? height(q->left) : 0;
            int right_height = (q->right != nullptr) ? height(q->right) : 0;
            q->height = 1 + max(left_height, right_height);
            q->size = 1 + size(q->left) + size(q->right);
            q = searchParent(T, q->key);
        }
    }
}

// 출력
void inorder(Node* tree) {
    if (tree == nullptr)
        return;  

    printf("<");               
    inorder(tree->left);       
    printf(" %d ", tree->key);     
    inorder(tree->right);         
    printf(">");               
}

int main() {
    char c;
    int key;
    Node* T = nullptr;
    // 입력 못받을 경우 중지
    while(scanf("%c %d", &c, &key) != EOF) {

        if (c == 'i') {
            if(searchBST(T, key) == nullptr){
                insertBST(T, key);
                inorder(T);
                printf("\n");
            }
            else{
                printf("i %d: The key already exists\n", key);
            }
            
        } else if (c == 'd') {
            if(searchBST(T, key) == nullptr){
                printf("d %d: The key does not exist\n", key);
            }
            else{
                eraseBST(T, key);
                inorder(T);
                printf("\n");
            }
        }
    }
    
    clear(T);
    
    return 0;
}