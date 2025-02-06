#include<iostream>
#include<stack>
using namespace std;
//Declare Node class
class Node{
    public:
    int* keys;
    Node** children;
    int size;

    Node() : size(0), keys(nullptr), children(nullptr) {}
    ~Node() {
        delete[] keys;
        delete[] children;
    }
};
// clear 함수
void clear(Node*& node) {
    if (node == nullptr) return;
    
    for (int i = 0; i <= node->size; i++) {
        clear(node->children[i]);
    }
    delete node;
}

// BTNode 만들기
Node* getBTNode(int m){
    Node* new_node = new Node();
    new_node->keys = new int[m-1];
    new_node->children = new Node*[m];
    for(int i=0;i<m;i++){
        if(i<m-1) new_node->keys[i] = 0;
        new_node->children[i] = nullptr;
    }
    return new_node;
}
//SearchPath 함수
pair<bool, stack<Node*> > searchPath(Node*& T, int key, stack<Node*>& s) {
  //트리가 null일 경우
    if (T == nullptr) {
      while(!s.empty()) s.pop();
    return make_pair(false, s);}
    
    Node* x = T;
    while (true) {
        int i = 0;
       
        while (i < x->size && key > x->keys[i]) 
            i++;
        
        s.push(x);
        
        if (i < x->size && key == x->keys[i]) 
            return make_pair(true, s);
            
        if (x->children[i] == nullptr)
            break;
            
        x = x->children[i];
    }
    return make_pair(false, s);
}

//insertkey 함수
void insertKey(int m, Node* x, Node* y, int newKey){
  if(x->size >= m-1) return;
  int i = x->size-1;
  while (i >= 0 && newKey < x->keys[i]){
    x->keys[i+1] = x->keys[i];
    x->children[i+2] = x->children[i+1];
    i--;
    }
  x->keys[i+1] = newKey;
  x->children[i+2] = y;
  x->size++;
}


//splitNode 함수
pair<int, Node*> splitNode(Node*& T, int m, Node* x, Node* y, int newKey){
  // tempNode는 임시노드 
  Node* tempNode = getBTNode(m + 1);
   // x의 데이터를 임시 노드로 복사
    for (int i = 0; i < x->size; i++) {
        tempNode->keys[i] = x->keys[i];
        tempNode->children[i] = x->children[i];
    }
    tempNode->children[x->size] = x->children[x->size];
    tempNode->size = x->size;
  insertKey( m+1, tempNode, y, newKey);

  int centerKey = tempNode->keys[tempNode->size / 2];

  x->size = 0;
  int i = 0;

  // 왼쪽 부분
  while (tempNode->keys[i] < centerKey){
    x->keys[i] = tempNode->keys[i];
    x->children[i] = tempNode->children[i];
    i++;
    x->size++;
    }
  x->children[i] = tempNode->children[i];

  Node* newNode = getBTNode(m);
  i++;
  // 오른쪽 부분
  while (i < tempNode->size) {
    newNode->keys[newNode->size] = tempNode->keys[i];
    newNode->children[newNode->size] = tempNode->children[i];
    i++;
    newNode->size++;
    }
  newNode->children[newNode->size] = tempNode->children[i];

  delete tempNode;
  return make_pair(centerKey, newNode);
  }


// deletekey 함수
void deleteKey(Node*& T, int m, Node*& x, int oldKey){
  int i = 0;
  while (i < x->size && x->keys[i] != oldKey) 
    i++;

  while (i < x->size) {
    x->keys[i] = x->keys[i + 1];
    x->children[i + 1] = x->children[i + 2];
    i++;
    }
  x->size--;
  }

// 형제 노드 중에 뺏어올 놈 찾기
int bestSibling(Node* x, Node* y){
  int i = 0;
  while (y->children[i] != x) {
    i++;
    }

  if (i == 0) 
    return i + 1;
  else if (i == y->size) 
    return i - 1;
  else if (y->children[i - 1]->size >= y->children[i + 1]->size)
    return i - 1;
  else return i + 1;
  }

// 키 재분배
void redistributeKeys(Node*& T, int m, Node*& x, Node*& y, int bestSib){
  int i = 0;
  while (y->children[i] != x)
    i++;

  Node* bestNode = y->children[bestSib];
  if (bestSib < i){
    int lastKey = bestNode->keys[bestNode->size - 1];
    insertKey(m, x, nullptr, y->keys[i - 1]);
    x->children[1] = x->children[0];
    x->children[0] = bestNode->children[bestNode->size];
    bestNode->children[bestNode->size] = nullptr;
    deleteKey(T, m, bestNode, lastKey);
    y->keys[i - 1] = lastKey;
    }
  else {
    int firstKey = bestNode->keys[0];
    insertKey(m, x, nullptr, y->keys[i]);
    x->children[x->size] = bestNode->children[0];
    bestNode->children[0] = bestNode->children[1];
    deleteKey(T, m, bestNode, firstKey);
    y->keys[i] = firstKey;
    }
}


// 노드 합치기
void mergeNode(Node*& T, int m, Node*& x, Node*& y, int bestSib){
  int i = 0;
  while (y->children[i] != x)
    i++;

  Node* bestNode = y->children[bestSib];
  if (bestSib > i){
    int tmp = i; i = bestSib; bestSib = tmp;
    Node* tmpnode = x; x = bestNode; bestNode = tmpnode;}
  bestNode->keys[bestNode->size] = y->keys[i - 1];
  bestNode->size++;
  int j = 0;
  while (j < x->size) {
    bestNode->keys[bestNode->size] = x->keys[j];
    bestNode->children[bestNode->size] = x->children[j];
    bestNode->size++;
    j++;
    }
  bestNode->children[bestNode->size] = x->children[x->size];
  deleteKey(T, m, y, y->keys[i - 1]);
  delete x;
  x = nullptr;
  }

// 노드 넣기
void insertBT(Node*& T, int m, int newKey){
    //트리 비어있을 때
  if (T == nullptr) {
    T = getBTNode(m);
    T->keys[0] = newKey;
    T->size = 1;

    return;
    }
    stack<Node*> s;
  pair<bool, stack<Node*> > temp = searchPath(T, newKey, s);
  bool found = temp.first;
  s = temp.second;
  if (found) return;

  bool finished = false;
    // x는 현재노드
  Node* x = s.top();
  s.pop();
  Node* y = nullptr;

  do{
    if (x->size < m - 1){
      insertKey(m, x, y, newKey);
      finished = true;}
    else {
      pair<int, Node*> temp = splitNode(T, m, x, y, newKey);
      int new_key = temp.first;
      y = temp.second;
      newKey = new_key;
      if (!s.empty()){
        x = s.top();
        s.pop();}
      else {
        Node* new_root = getBTNode(m);
        new_root->keys[0] = new_key;
        new_root->children[0] = x;
        new_root->children[1] = y;
        new_root->size = 1;
        T = new_root;
        finished = true;}}}
  while (!finished);
  }

// 여기가 leaf노드인지 검사
bool checkLeafNode(Node* T){
  if(T == nullptr) return false;
  for(int i=0;i<=T->size;i++){
    if(T->children[i] != nullptr){
      return false;
    }
  }
  return true;
}

void deleteBT(Node*& T, int m, int oldKey){
  stack<Node*> s;
  pair<bool, stack<Node*> > temp = searchPath(T, oldKey, s);
  bool found = temp.first;
  s = temp.second;
  if (found == false) return;

  Node* x = s.top();
  s.pop();
  Node* y = nullptr;
//leaf 노드가 아니면
  if (!checkLeafNode(x)){
    Node* internalNode = x;
		// int i <- index of oldKey in internalNode;
    int i = 0;
    while(i < x->size && x->keys[i] != oldKey) i++;


		s.push(x);
    searchPath(x->children[i + 1], x->keys[i], s);

    x = s.top();
    s.pop();
    
        
    int temp = internalNode->keys[i];
    internalNode->keys[i] = x->keys[0];
    x->keys[0] = temp; 
        }

  bool finished = false;
  deleteKey(T, m, x, oldKey);

  if (!s.empty()){
    y = s.top();
    s.pop();
    }

  do{
    // if(x == nullptr) break;
    if (T == x || x->size >= (m - 1) / 2)
      finished = true;
    else {
      int bestSib = bestSibling(x, y);

      if (y->children[bestSib]->size > (m - 1) / 2){
        redistributeKeys(T, m, x, y, bestSib);
        finished = true;}
      else {
        mergeNode(T, m, x, y, bestSib);
        x = y;
        if (!s.empty()){
          y =s.top();
          s.pop();}
        else
          finished = true;}}}
  while (!finished);

  if (y != nullptr && y->size == 0){
    T = y->children[0];
    delete y;}

  if(T != nullptr && T->size ==0){
    delete T;
    T = nullptr;
  }
    }

void inorder(Node* tree) {
    
    if (tree == nullptr)
        return;  

    printf("<"); 
    
    for(int i=0;i<tree->size;i++){
        if(tree->children[i] != nullptr){
            inorder(tree->children[i]);
        }
        // if (i < tree->size - 1 && cnt == 0) {printf(" "); cnt = 1;
        printf(" %d", tree->keys[i]);
        if(tree->children[i+1] != nullptr) printf(" ");
        // if(i == tree->size-1) printf(" ");
    }      
    
    if(tree->children[tree->size] != nullptr){
        
        inorder(tree->children[tree->size]);
    } 
    else{
      printf(" ");
    }
    
    printf(">");
    
                   
}

int main() {
    char c;
    int key;
    Node* T = nullptr;
    stack<Node*> s;
    // 입력 못받을 경우 중지
    while(scanf(" %c %d", &c, &key) != EOF) {
        if (c == 'i') {
            if(searchPath(T, key, s).first == false) {
                insertBT(T, 4, key);
                inorder(T);
                printf("\n");
            } else {
                printf("i %d: The key already exists\n", key);
            }
        } else if (c == 'd') {
            if(searchPath(T, key, s).first == false) {
                  printf("d %d: The key does not exist\n", key);
              } else {
                deleteBT(T, 4, key);
                if(T == nullptr) break;
                inorder(T);
                printf("\n");
            }
        } else {
            // 유효하지 않은 명령일 경우
            printf("Invalid command: %c\n", c);
            return 1; 
        }
    }
    // inorder_debug(T, 4);
    // 메모리 정리
    clear(T);
    
    return 0;
}