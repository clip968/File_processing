#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    int size; 
    int bf;
    Node() : left(nullptr), right(nullptr), height(1), size(1), bf(0){}
};

struct BalanceValues {
    int rotateType;
    Node* a;
    Node* f;
};

Node* searchBST(Node* T, int searchKey) {
    if (T == nullptr) return nullptr;

    if (searchKey == T->key) return T;
    if (searchKey < T->key) return searchBST(T->left, searchKey);

    else return searchBST(T->right, searchKey);
}

// Node 생성
Node* getNodeAVL() {
    Node* newNode = new Node();
    return newNode;
}
void clear(Node*& T){
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

Node* getBSTNode() {
    Node* newNode = new Node();
    return newNode;
}
void updateNode(Node*& T){
    if(T == nullptr) return;
    updateNode(T->left);
    updateNode(T->right);
    int left_height = (T->left != nullptr) ? height(T->left) : 0;
    int right_height = (T->right != nullptr) ? height(T->right) : 0;
    T->height = 1 + max(left_height, right_height);
    T->size = 1 + size(T->left) + size(T->right);
    T->bf = left_height - right_height;
    
}
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


BalanceValues checkBalance(Node* &T, int newKey){
    BalanceValues values;
    int rotateType = 0;
    values.rotateType = 0;
    values.a = nullptr;
    values.f = nullptr;

    // 반환할 불균형 노드의 부모를 저장할 포인터

    Node* f = nullptr;

    // 반환할 불균형 노드를 저장할 포인터
    Node* a = T;

    // newKey가 삽입된, 불균형 노드 a의 자식을 저장할 포인터

    // Step 1: 새로 삽입된 newKey의 위치 q를 찾음

    Node* p = T;
    Node* q = nullptr;

    while (p != nullptr)  {
        if (p->bf != 0) { a = p; f = q; }
        if (newKey < p->key) { q = p; p = p->left; }
        else if (newKey > p->key){ q = p; p = p->right; }
        else { q = p; break; }
    }
   
    
    // Step 2: BF 계산
    
    
        if(1 < q->bf){
            if(p->left->bf > 0) rotateType = 1;
            else rotateType = 2;
        }
        else{
            if(p->right->bf > 0) rotateType = 3;
            else rotateType = 4;
        }

    values.rotateType = rotateType;
    values.a = a;
    values.f = f;
    return values;
}

void rotateTree(Node* &T, int rotateType, Node* &p, Node* &q){
    Node* a = nullptr;
    Node* b = nullptr;
    Node* c = nullptr;
    //rotationType = 0 -> No, 1 -> LL, 2 -> LR, 3 -> RL 4 -> RR
    //LL
    if (rotateType == 1) {
        a = p;
        b = p->left;
        a->left = b->right;
        b->right = a;
        a->bf = 0; b->bf = 0;
    } else if (rotateType == 2) {
        a = p;
        // if(a == nullptr || a->left == nullptr) return;
        b = p->left; c = b->right;
        if(c != nullptr){
            b->right = c->left;
             a->left = c->right;
        c->left = b; c->right = a;
        switch (c->bf) {
            case 0 : b->bf = 0; a->bf = 0; break;
            case 1 : a->bf = -1; b->bf = 0; break;
            case -1 : b->bf = 1; a->bf = 0; break;
    }
    c->bf = 0; b = c;
    }
   
     }
       
    else if (rotateType == 4){
        a = p;
        b = p->right;
        a->right = b->left;
        b->left = a;
        a->bf = 0; b->bf = 0;
    }
    else if(rotateType == 3){
        a = p; 
        // if(a == nullptr || a->right == nullptr) return;
        b = p->right; c = b->left;
        if(c != nullptr){
        b->left = c->right;
        a->right = c->left;
        c->right = b; c->left = a;
        switch(c->bf){
            case 0 : b->bf = 0; a->bf = 0; break;
            case 1 : a->bf = 0; b->bf = -1; break;
            case -1 : a->bf = 1; b->bf = 0; break;
        }
        c->bf = 0; b = c;
        }
        // 부모 자식 관계 수정
        }

    
    if (q == nullptr) T = b;
    // b를 루트로 하는 서브트리를 q의 자식으로 함
    else if (a == q->left) q->left = b;
    else if (a == q->right) q->right = b;
    updateNode(T);
}

// 노드 넣기
Node* insertBST(Node*& T, int newKey) {
    
    // create new node
    Node* newNode = getNodeAVL();
    newNode->key = newKey;
    
    updateNode(T);
    return newNode;
}
//노드 찾기


Node* eraseBST(Node*& T, int deleteKey) {
    Node* p = T;
    Node* q = nullptr;
   

    // find position of deleteKey while storing parent node on stack
    while (p != nullptr && deleteKey != p->key) { 
        q = p;
        

        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    if (p == nullptr) return nullptr;  // deleteKey was not found

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
        if(iflag == 0) return eraseBST(p->left, trash->key);
        else return eraseBST(p->right, trash->key);
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
        updateNode(T);
    }
    return q;
}

void deleteAVL(Node*& T, int deleteKey){

    Node* p = nullptr;
    Node* q = eraseBST(T, deleteKey);
    Node* x = nullptr;
    
    updateNode(T);
    
    
    while (q != nullptr) {
        updateNode(T);

    if (1 < q->bf || q->bf < -1)
      if (x == nullptr){
        x = q;
        }
        q = searchParent(T, q->key);
      }

  // if there's no problem, return
  
    
    if (x == nullptr){
        return;
    }
    
        
        BalanceValues values;
        values = checkBalance(T, x->key);
        int rotationType = values.rotateType;
        p = values.a;
        q = values.f;
        // rotationType, p, q ← checkBalance(T, a.key); // p: 불균형 위치, q: p의 부모
        // 균형 트리면, “NO”를 리턴
        // Step 3: rebalancing 실행
        
        if (rotationType != 0) {
        rotateTree(T, rotationType, p, q);
        
        } 
}

void insertAVL(Node*& T, int newKey){
  Node* p = T;
  Node* q = nullptr;
  Node* x = nullptr;
    if (T == nullptr) {
        Node* y = getNodeAVL();
        // getNodeBST()에서 균형인수 bf를 추가
        y->key = newKey; ;
        T = y;
        return;
    }
    else{
        Node* y = insertBST(T, newKey);
        // y->key = newKey;
        while (p != nullptr) {
    if (newKey == p->key) return;
    
    q = p;
    
    if (newKey < p->key) p = p->left;
    else p = p->right;
  }

  // create new node
  // insert y as a child of q
  if (T == nullptr) T = y;
  else if (newKey < q->key) q->left = y;
  else q->right = y;

  // update height and balancing factor while popping parent node from stack
  while (q != nullptr){ 
    updateNode(T);

    if (q->bf < -1 || 1 < q->bf){
      if (x == nullptr){
        x = q;
        
      }
    }
    q = searchParent(T, q->key); // 추가: 부모 노드를 추적하며 반복
  }

  // if there's no problem, return
  if (x == nullptr) return;

  // rebalance tree
  else{
    BalanceValues values;
    values = checkBalance(T, x->key);
    int rotationType = values.rotateType;
    Node* p = values.a;
    Node* q = values.f;

    // Step 3: rebalancing 실행
    if (rotationType != 0) {
      rotateTree(T, rotationType, p, q);
        // updateNode(T);
      
    }
  }
  
}
    }
  // find position to insert newKey while storing parent node on stack
  


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
                insertAVL(T, key);
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
                deleteAVL(T, key);
                inorder(T);
                printf("\n");
            }
        }
    }
    
    clear(T);
    return 0;
} 