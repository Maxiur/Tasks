#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <vector>
#include <optional>
#include <random>
#include <bits/stdc++.h>
using namespace std;

struct Entry {
    int key;
    int value;
    bool deleted = false;
};

class OpenAddressingHashTable {
private:
    vector<optional<Entry>> table;
    int size;

    int hash(int key) {
        return (key % size + size) % size;
    }

public:
    OpenAddressingHashTable(int size) : size(size), table(size) {}

    void insert(int key, int value) {
        int index = hash(key);
        int start_index = index;

        while (table[index].has_value()) {
            if (!table[index]->deleted && table[index]->key == key) {
                table[index]->value = value;
                return;
            }
            if (table[index]->deleted) break;

            index = (index + 1) % size;
            if (index == start_index) {
                cout << "Table is full" << endl;
                return;
            }
        }
        table[index] = Entry{key, value, false};
    }

    optional<int> search(int key) {
        int index = hash(key);
        int start_index = index;

        while (table[index].has_value()) {
            if (!table[index]->deleted && table[index]->key == key) {
                return table[index]->value;
            }
            index = (index + 1) % size;
            if (index == start_index) break;
        }
        return {};
    }

    bool remove(int key) {
        int index = hash(key);
        int start_index = index;

        while (table[index].has_value()) {
            if (!table[index]->deleted && table[index]->key == key) {
                table[index]->deleted = true;
                return true;
            }
            index = (index + 1) % size;
            if (index == start_index) break;
        }
        return false;
    }
};

class ChainingHashTable {
private:
    vector<list<Entry>> table;
    int size;

    int hash(int key) {
        return (key % size + size) % size;
    }

public:
    ChainingHashTable(int size) : size(size), table(size) {}

    void insert(int key, int value) {
        int index = hash(key);
        for (auto &e : table[index]) {
            if (e.key == key) {
                e.value = value;
                return;
            }
        }
        table[index].push_back(Entry{key, value});
    }

    optional<int> search(int key) {
        int index = hash(key);
        for (auto &e : table[index]) {
            if (e.key == key) return e.value;
        }
        return {};
    }

    bool remove(int key) {
        int index = hash(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                table[index].erase(it);
                return true;
            }
        }
        return false;
    }
};

struct TreeNode {
    int value;
    TreeNode *left;
    TreeNode *right;

    explicit TreeNode(int value, TreeNode *left = nullptr, TreeNode *right = nullptr) : value(value), left(left), right(right) {}
};

class BST {
private:
    TreeNode *root;

    TreeNode* findMin(TreeNode *node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode* removeNode(TreeNode *node, int value) {
        if (node == nullptr) return nullptr;

        if (value < node->value)
            node->left = removeNode(node->left, value);
        else if (value > node->value)
            node->right = removeNode(node->right, value);
        else {
            if (node->left == nullptr) {
                TreeNode* rightChild = node->right;
                delete node;
                return rightChild;
            }
            else if (node->right == nullptr) {
                TreeNode* leftChild = node->left;
                delete node;
                return leftChild;
            }

            TreeNode* successor = findMin(node->right);
            node->value = successor->value;
            node->right = removeNode(node->right, successor->value);

        }
        return node;
    }


public:
    BST() {
        root = nullptr;
    }

    void insert(int value) {
        if (root == nullptr) {
            root = new TreeNode(value);
            return;
        }

        TreeNode *current = root;
        TreeNode *parent = nullptr;
        while (current != nullptr) {
            parent = current;
            if (current->value > value) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        if (value < parent->value)
            parent->left = new TreeNode(value);
        else
            parent->right = new TreeNode(value);
    }

    TreeNode *search(int value) {
        TreeNode *current = root;
        while (current != nullptr) {
            if (value == current->value)
                return current;
            else if (current->value > value) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return nullptr;
    }

    void remove(int value) {
        root = removeNode(root, value);
    }
};

struct AVLNode {
    int key, height;
    AVLNode *left, *right;
    AVLNode(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

class AVL {
    AVLNode* root = nullptr;

    int height(AVLNode* node) { return node ? node->height : 0; }
    int balance(AVLNode* node) { return node ? height(node->left) - height(node->right) : 0; }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    AVLNode* insertRec(AVLNode* node, int k) {
        if (!node) return new AVLNode(k);
        if (k < node->key) node->left = insertRec(node->left, k);
        else if (k > node->key) node->right = insertRec(node->right, k);
        else return node;

        node->height = 1 + std::max(height(node->left), height(node->right));
        int b = balance(node);

        if (b > 1 && k < node->left->key) return rotateRight(node);        // LL
        if (b < -1 && k > node->right->key) return rotateLeft(node);       // RR
        if (b > 1 && k > node->left->key) { node->left = rotateLeft(node->left); return rotateRight(node); } // LR
        if (b < -1 && k < node->right->key) { node->right = rotateRight(node->right); return rotateLeft(node); } // RL

        return node;
    }

    AVLNode* minNode(AVLNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    AVLNode* removeRec(AVLNode* node, int k) {
        if (!node) return nullptr;
        if (k < node->key) node->left = removeRec(node->left, k);
        else if (k > node->key) node->right = removeRec(node->right, k);
        else {
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                if (!temp) { temp = node; node = nullptr; }
                else *node = *temp;
                delete temp;
            } else {
                AVLNode* temp = minNode(node->right);
                node->key = temp->key;
                node->right = removeRec(node->right, temp->key);
            }
        }

        if (!node) return node;

        node->height = 1 + std::max(height(node->left), height(node->right));
        int b = balance(node);

        if (b > 1 && balance(node->left) >= 0) return rotateRight(node);
        if (b > 1 && balance(node->left) < 0) { node->left = rotateLeft(node->left); return rotateRight(node); }
        if (b < -1 && balance(node->right) <= 0) return rotateLeft(node);
        if (b < -1 && balance(node->right) > 0) { node->right = rotateRight(node->right); return rotateLeft(node); }

        return node;
    }

    bool searchRec(AVLNode* node, int k) {
        if (!node) return false;
        if (k == node->key) return true;
        return k < node->key ? searchRec(node->left, k) : searchRec(node->right, k);
    }

public:
    void insert(int k) { root = insertRec(root, k); }
    bool search(int k) { return searchRec(root, k); }
    void remove(int k) { root = removeRec(root, k); }
};

struct Node {
    int key;
    Node *left, *right, *parent;
    Node(int k) : key(k), left(nullptr), right(nullptr), parent(nullptr) {}
};

class SplayTree {
private:
    Node* root;

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right) y->right->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else y->right = x;
        x->parent = y;
    }

    void splay(Node* x) {
        while (x->parent) {
            if (!x->parent->parent) { // Zig
                if (x == x->parent->left) rotateRight(x->parent);
                else rotateLeft(x->parent);
            } else if (x == x->parent->left && x->parent == x->parent->parent->left) { // Zig-Zig
                rotateRight(x->parent->parent);
                rotateRight(x->parent);
            } else if (x == x->parent->right && x->parent == x->parent->parent->right) { // Zig-Zig
                rotateLeft(x->parent->parent);
                rotateLeft(x->parent);
            } else if (x == x->parent->left && x->parent == x->parent->parent->right) { // Zig-Zag
                rotateRight(x->parent);
                rotateLeft(x->parent);
            } else { // Zag-Zig
                rotateLeft(x->parent);
                rotateRight(x->parent);
            }
        }
    }

public:
    SplayTree(): root(nullptr) {}

    void insert(int k) {
        if (!root) {
            root = new Node(k);
            return;
        }
        Node* curr = root;
        Node* prev = nullptr;
        while (curr) {
            prev = curr;
            if (k < curr->key) curr = curr->left;
            else if (k > curr->key) curr = curr->right;
            else { splay(curr); return; }
        }
        Node* newNode = new Node(k);
        newNode->parent = prev;
        if (k < prev->key) prev->left = newNode;
        else prev->right = newNode;
        splay(newNode);
    }

    bool search(int k) {
        Node* curr = root;
        while (curr) {
            if (k < curr->key) curr = curr->left;
            else if (k > curr->key) curr = curr->right;
            else { splay(curr); return true; }
        }
        return false;
    }

    void remove(int k) {
        if (!search(k)) return;
        Node* oldRoot = root;
        if (!root->left) {
            root = root->right;
            if (root) root->parent = nullptr;
        } else {
            Node* leftSubtree = root->left;
            leftSubtree->parent = nullptr;
            Node* m = leftSubtree;
            while (m->right) m = m->right;
            root = leftSubtree;
            splay(m);
            root->right = oldRoot->right;
            if (oldRoot->right) oldRoot->right->parent = root;
        }
        delete oldRoot;
    }
};

// ------------------ Benchmark dla hash table ------------------
template<typename HashTable>
void benchmarkHash(HashTable &ht, const string &name, const vector<int> &data) {
    auto start = chrono::high_resolution_clock::now();
    for(int x : data) ht.insert(x, x);
    auto end = chrono::high_resolution_clock::now();
    cout << name << " insert: " << duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    for(int x : data) ht.search(x);
    end = chrono::high_resolution_clock::now();
    cout << name << " search: " << duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    for(int x : data) ht.remove(x);
    end = chrono::high_resolution_clock::now();
    cout << name << " remove: " << duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
}

// ------------------ Benchmark dla drzew ------------------
template<typename Tree>
void benchmarkTree(Tree &tree, const string &name, const vector<int> &data) {
    auto start = chrono::high_resolution_clock::now();
    for(int x : data) tree.insert(x);
    auto end = chrono::high_resolution_clock::now();
    cout << name << " insert: " << duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    for(int x : data) tree.search(x);
    end = chrono::high_resolution_clock::now();
    cout << name << " search: " << duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    for(int x : data) tree.remove(x);
    end = chrono::high_resolution_clock::now();
    cout << name << " remove: " << duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
}

// ------------------ Uruchomienie benchmarku ------------------
void runBenchmark(int N) {
    OpenAddressingHashTable hashOA(N*2);
    ChainingHashTable hashChain(N/2);
    BST bst;
    AVL avl;
    SplayTree spt;

    vector<int> data(N);
    iota(data.begin(), data.end(), 1);
    ranges::shuffle(data.begin(), data.end(), default_random_engine{});

    cout << "--- Benchmark ---\n";
    benchmarkHash(hashOA, "Hash OA", data);
    benchmarkHash(hashChain, "Hash Chain", data);

    benchmarkTree(bst, "BST", data);
    benchmarkTree(avl, "AVL", data);
    benchmarkTree(spt, "Splay Tree", data);
}

int main() {
    runBenchmark(100000);
}

// --- Benchmark ---
// Hash OA insert: 6 ms
// Hash OA search: 4 ms
// Hash OA remove: 3 ms
// Hash Chain insert: 22 ms
// Hash Chain search: 21 ms
// Hash Chain remove: 23 ms
// BST insert: 18 ms
// BST search: 14 ms
// BST remove: 17 ms
// AVL insert: 47 ms
// AVL search: 16 ms
// AVL remove: 48 ms
// Splay Tree insert: 5 ms
// Splay Tree search: 0 ms
// Splay Tree remove: 0 ms
