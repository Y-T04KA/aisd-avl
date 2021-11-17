#include <iostream>
#include <queue>

using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
};

int max(int a, int b) {//get max of arguments
    if (a >= b) return a;
    else if (b > a) return b;
};

int height(Node* N) {
    if (N == NULL) return 0;
    return N->height;
}

Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node* N) {
    if (N == NULL) return 0;
    return height(N->left) - height(N->right);
}

Node* insert(Node* node, int key) {//а это
    if (node == NULL) return(newNode(key));
    if (key < node->key) node->left = insert(node->left, key);
    else if (key > node->key) node->right = insert(node->right, key);
    else return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)return rightRotate(node);
    if (balance < -1 && key > node->right->key) return leftRotate(node);
    if (balance > 1 && key > node->left->key){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->key){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void printNodeNLR(const Node* node, int key, int height, int prob){//prob - костыль для древоподобного вывода
    if (node){
        if (height == key){//тут мы центральную ноду мучаем
            for (int i = 1; i <= prob; i++)
                cout << " ";
            cout << node->key;
        }
        else{
            printNodeNLR(node->left, key, height + 1, prob);//тут мы рекурсивно отправляемся в левую ветку
            printNodeNLR(node->right, key, height + 1, prob);//тут мы отправляемся в правую
        }//по идее, чтобы перейти к центрированному и обратному обходам надо просто поменять их местами
    }
    else
        cout << "     ";
}

void NLR(Node* node)
{
    int h = height(node);
    int prob = 4;
    if (node)
    {
        for (int i = 0; i <= h; i++)
        {
            printNodeNLR(node, i, 0, prob * (h - i));
            //printNode(node, height(node));
            cout << endl;
        }
    }
}

void printNodeLNR(const Node* node, int key, int height, int prob) {//prob - костыль для древоподобного вывода
    if (node) {
        printNodeLNR(node->left, key, height + 1, prob);//тут мы рекурсивно отправляемся в левую ветку
        if (height == key) {//тут мы центральную ноду мучаем
            for (int i = 1; i <= prob; i++)
                cout << " ";
            cout << node->key;
        }
        else {
            
            printNodeLNR(node->right, key, height + 1, prob);//тут мы отправляемся в правую
        }//по идее, чтобы перейти к центрированному и обратному обходам надо просто поменять их местами
    }
    else
        cout << "     ";
}

void LNR(Node* node)
{
    int h = height(node);
    int prob = 4;
    if (node)
    {
        for (int i = 0; i <= h; i++)
        {
            printNodeLNR(node, i, 0, prob * (h - i));
            //printNode(node, height(node));
            cout << endl;
        }
    }
}

void printNodeLRN(const Node* node, int key, int height, int prob) {//prob - костыль для древоподобного вывода
    if (node) {
        printNodeLRN(node->left, key, height + 1, prob);//тут мы рекурсивно отправляемся в левую ветку
        printNodeLRN(node->right, key, height + 1, prob);//тут мы отправляемся в правую
        if (height == key) {//тут мы центральную ноду мучаем
            for (int i = 1; i <= prob; i++)
                cout << " ";
            cout << node->key;
        }
        //по идее, чтобы перейти к центрированному и обратному обходам надо просто поменять их местами
    }
    else
        cout << "     ";
}

void LRN(Node* node)
{
    int h = height(node);
    int prob = 4;
    if (node)
    {
        for (int i = 0; i <= h; i++)
        {
            printNodeLNR(node, i, 0, prob * (h - i));
            cout << endl;
        }
    }
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL) current = current->left;
    return current;
}

Node* deleteNode(Node* root, int key) {//а поиск ноды для удаления считается за поиск?
    if (root == NULL) return root;
    if (key < root->key) root->left = deleteNode(root->left, key);
    else if (key > root->key) root->right = deleteNode(root->right, key);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else *root = *temp;
            free(temp);
        }
        else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    if (root == NULL) return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0){
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0){
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

int main()
{
    Node* root = NULL;
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);
    cout << "NLR Tree is\n";
    NLR(root);
    cout << "LNR Tree is\n";
    LNR(root);
    //cout << "LRN Tree is\n";
    root = deleteNode(root, 25);
    cout << "\nNew Tree via LRN is\n"; //TODO BFS
    LRN(root);
    return(0);
}