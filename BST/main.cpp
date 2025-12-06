#include <iostream>

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

int main() {
    BST tree;

    // --- INSERT ---
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(20);

    // --- SEARCH ---
    std::cout << "Szukam 7: ";
    std::cout << (tree.search(7) ? "Znaleziono\n" : "Brak\n");

    std::cout << "Szukam 999: ";
    std::cout << (tree.search(999) ? "Znaleziono\n" : "Brak\n");

    // --- DELETE ---
    tree.remove(3);   // liść
    tree.remove(15);  // jeden syn (12)
    tree.remove(10);  // dwójka dzieci

    // --- SEARCH ---
    std::cout << "Po usunieciach:\n";
    std::cout << "Szukam 3: "   << (tree.search(3)   ? "Jest\n" : "Brak\n");
    std::cout << "Szukam 15: "  << (tree.search(15)  ? "Jest\n" : "Brak\n");
    std::cout << "Szukam 10: "  << (tree.search(10)  ? "Jest\n" : "Brak\n");
    std::cout << "Szukam 12: "  << (tree.search(12)  ? "Jest\n" : "Brak\n");
    std::cout << "Szukam 20: "  << (tree.search(20)  ? "Jest\n" : "Brak\n");
}