#include <iostream>

template <typename T>
struct ListNode {
    ListNode *next;
    T val;

    ListNode(const T& x, ListNode *next) : next(next), val(x) {}
    ListNode(const T& x) : next(nullptr), val(x) {}
    ListNode() : next(nullptr), val(T{}) {}
    ~ListNode() = default;
};

template <typename T>
class PriorityQueue {
public:
    ListNode<T> *head;

    PriorityQueue() {
        head = new ListNode<T>();
    }

    PriorityQueue(const PriorityQueue &other) {
        head = new ListNode<T>();
        ListNode<T>* curr = head;
        ListNode<T>* otherCurr = other.head->next;
        while (otherCurr != nullptr) {
            curr->next = new ListNode<T>(otherCurr->val);
            curr = curr->next;
            otherCurr = otherCurr->next;
        }
    }

    PriorityQueue& operator=(const PriorityQueue &other) {
        if (this == &other) return *this;

        ListNode<T>* curr = head->next;
        while (curr) {
            ListNode<T>* next = curr->next;
            delete curr;
            curr = next;
        }
        head->next = nullptr;

        curr = head;
        ListNode<T>* otherCurr = other.head->next;
        while (otherCurr) {
            curr->next = new ListNode<T>(otherCurr->val);
            curr = curr->next;
            otherCurr = otherCurr->next;
        }
        return *this;
    }

    ~PriorityQueue() {
        ListNode<T> *curr = head;
        while (curr != nullptr) {
            ListNode<T> *next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void insert(T val) {
        ListNode<T> *prev = find_last(val);

        // if exists
        if (prev->next != nullptr && prev->next->val == val)
            return;

        prev->next = new ListNode<T>(val, prev->next);
    }

    void remove(T val) {
        ListNode<T> *prev = find_last(val);
        if (prev->next != nullptr && prev->next->val == val) {
            ListNode<T> *toDelete = prev->next;
            prev->next = toDelete->next;
            delete toDelete;
        }
    }

    bool contains(T val) const {
        ListNode<T> *curr = find_last(val);
        return curr->next != nullptr && curr->next->val == val;
    }

    T popMin() {
        ListNode<T>* curr = head->next;
        if (curr == nullptr) throw std::runtime_error("Priority queue is empty");

        T minVal = curr->val;

        head->next = curr->next;
        delete curr;

        return minVal;
    }

private:
    // finding prev pointer of desired value
    ListNode<T>* find_last(T val) const {
        ListNode<T> *curr = head;
        while (curr->next != nullptr && curr->next->val < val) {
            curr = curr->next;
        }
        return curr;
    }
};

int main() {
    // --- TEST 1: LICZBY CAŁKOWITE ---
    std::cout << "--- Test Intów ---" << std::endl;
    PriorityQueue<int> pqInt;

    pqInt.insert(50);
    pqInt.insert(10);
    pqInt.insert(30);
    pqInt.insert(10); // Duplikat
    pqInt.insert(5);

    std::cout << "PopMin (powinno byc 5): " << pqInt.popMin() << std::endl;
    std::cout << "PopMin (powinno byc 10): " << pqInt.popMin() << std::endl;
    std::cout << "Contains 30? " << (pqInt.contains(30) ? "Tak" : "Nie") << std::endl;

    // --- TEST 2: NAPISY (ALFABETYCZNIE) ---
    std::cout << "\n--- Test Stringów ---" << std::endl;
    PriorityQueue<std::string> pqStr;

    pqStr.insert("Zebra");
    pqStr.insert("Aligator");
    pqStr.insert("Mysz");

    std::cout << "Najmniejszy string (popMin): " << pqStr.popMin() << std::endl;
    std::cout << "Kolejny (popMin): " << pqStr.popMin() << std::endl;

    return 0;
}
