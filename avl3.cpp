#include <iostream>
using namespace std;

// 노드 구조체
class Node {
    public:
    int key;
    Node* left;
    Node* right;
    int height;
    int size; 
    int bf;
    public:
    Node() : left(nullptr), right(nullptr), height(1), size(1), bf(0){}
};
// BalanceValues 구조체
struct BalanceValues {
    int rotateType;
    Node* a;
    Node* f;
};
//노드 찾기
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

    T = nullptr;
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

// 기존 BSTNode 함수
Node* getBSTNode() {
    Node* newNode = new Node();
    return newNode;
}
// 부모 Node 찾기
Node* searchParent(Node* T, int searchKey){
    if(T == nullptr || T->key == searchKey) return nullptr;
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
// 부모 노드 올라가면서 업데이트
void updateNode(Node*& T, Node* update_node){
    if(update_node == nullptr) return;
    
    while(update_node != nullptr){
        int left_height = (update_node->left != nullptr) ? height(update_node->left) : 0;
        int right_height = (update_node->right != nullptr) ? height(update_node->right) : 0;
        update_node->height = 1 + max(left_height, right_height);
        update_node->size = 1 + size(update_node->left) + size(update_node->right);
        update_node->bf = left_height - right_height;
        //부모 노드로 올라가기
        update_node = searchParent(T, update_node->key);
    }
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
        else if(q->bf < -1){
            if(p->right->bf < 0) rotateType = 4;
            else rotateType = 3;
        }
    

        
    // Step 3: rotateType, a, f 반환
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
   if (rotateType == 1) {
    // LL
        a = p;
        b = p->left;
        a->left = b->right;
        b->right = a;
        updateNode(T, a);
        updateNode(T, b);
        
    } else if (rotateType == 2) {
        // LR
        a = p;
        b = p->left; c = b->right;
        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;
        updateNode(T, a);
        updateNode(T, b);
        b = c;
        
     }
       
    else if (rotateType == 4){
        // RR
       a = p; 
        b = p->right;
        a->right = b->left;
        b->left = a;
        updateNode(T, a);
        updateNode(T, b);
        
    }
    else if(rotateType == 3){
        // RL
        a = p;
        b = p->right; c = b->left;
        b->left = c->right; a->right = c->left;
        c->right = b; c->left = a;
        updateNode(T, b);
        updateNode(T, a);
        b = c;
        
        }

    
    if (q == nullptr) T = b;
    // b를 루트로 하는 서브트리를 q의 자식으로 함
    else if (a == q->left) q->left = b;
    else if (a == q->right) q->right = b;
    updateNode(T, b);
    
    
}

// insertBST 알고리즘
Node* insertBST(Node*& T, int newKey) {
    Node* p = T;
    Node* q = nullptr;
    Node* x = nullptr;

    // find position to insert newKey while storing parent node on stack
    while (p != nullptr) {
        if (newKey == p->key) return nullptr; // newKey already exists in T

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
    
    // q 부모노드 따라 올라가면서 루트노드까지
    while (q != nullptr){ 
        // q에서부터 루트까지 업데이트
        updateNode(T, q);
        if(q == nullptr) return nullptr;
        // q 불균형 검사
        if (q->bf < -1 || 1 < q->bf){
        if (x == nullptr){
            x = q;
        }
        }
        // q의 부모노드로 
        q = searchParent(T, q->key); 
    }
    return x;
}

// BST 삭제 알고리즘
Node* eraseBST(Node*& T, int deleteKey) {
    Node* p = T;
    Node* q = nullptr;
   

    // find position of deleteKey while storing parent node on stack
    while (p != nullptr && deleteKey != p->key) { 
        q = p;
        

        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    // if (p == nullptr) return nullptr;  // deleteKey was not found

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
        if(iflag == 0) q = eraseBST(p->left, trash->key);
        else q = eraseBST(p->right, trash->key);
        // p가 루트 노드일 경우 eraseBST는 nullptr 값을 반환함. 그걸 방지하는 용도
        if (q == nullptr) q = p;
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
        //p 삭제
        delete p;
        
    }   
    // p의 부모 노드 반환
    updateNode(T, q);
    return q;
}

// AVL 삭제
void eraseAVL(Node*& T, int deleteKey){
    Node* p = nullptr;
    Node* q = eraseBST(T, deleteKey);
    Node* f = nullptr;
    
    
    
    if(q == nullptr) return;
    while(q != nullptr) {
        if(1 < q->bf || q->bf < -1){
            // updateNode(T, q);
            BalanceValues values;
            values = checkBalance(T,q->key);
            int rotationType = values.rotateType;
            p = values.a;
            f = values.f;
        // rotationType, p, q ← checkBalance(T, a.key); // p: 불균형 위치, q: p의 부모
        // 균형 트리면, “NO”를 리턴
        // Step 3: rebalancing 실행
        
        if (rotationType != 0) {
            rotateTree(T, rotationType, p, f);
            
        } 
        }
        
        q = searchParent(T, q->key);
    }
            
    }
        
// 삽입
void insertAVL(Node*& T, int newKey){

 if (T == nullptr) {
        Node* y = getNodeAVL();
        // getNodeBST()에서 균형인수 bf를 추가
        y->key = newKey; y->left = nullptr; y->right = nullptr; y->bf = 0;
        T = y;
        return;
    }
  else{
    Node* x = insertBST(T, newKey);
    

  // if there's no problem, return
  if (x == nullptr) return;

  // rebalance tree
  else{
    BalanceValues values;
    // 균형 검사
    values = checkBalance(T, x->key);
    // rotationType, p, q ← checkBalance(T, a.key); // p: 불균형 위치, q: p의 부모
    int rotationType = values.rotateType;
    Node* p = values.a;
    Node* f = values.f;

    // rebalancing 실행
    if (rotationType != 0) {
        // T = 트리, p = 불균형 위치, q = p의 부모
        // rotateTree에서 좀 더 간편하게 인자를 전달하기 위하여 직접 p, q, rotationType을 전달함 
      rotateTree(T, rotationType, p, f);
      
    }
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
    while(scanf(" %c %d", &c, &key) != EOF) {
        if (c == 'i') {
            if(searchBST(T, key) == nullptr) {
                insertAVL(T, key);
                inorder(T);
                printf("\n");
            } else {
                printf("i %d: The key already exists\n", key);
            }
        } else if (c == 'd') {
            if(searchBST(T, key) == nullptr) {
                printf("d %d: The key does not exist\n", key);
            } else {
                eraseAVL(T, key);
                inorder(T);
                printf("\n");
            }
        } else {
            // 유효하지 않은 명령일 경우
            printf("Invalid command: %c\n", c);
            return 1; // 프로그램 종료
        }
    }

    // 메모리 정리
    clear(T);
    
    return 0;
}