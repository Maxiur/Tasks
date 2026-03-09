#include <iostream>

struct ListNode {
    ListNode *next;
    int val;

    ListNode(const int x, ListNode *next) : next(next), val(x) {}
    ListNode(const int x) : next(nullptr), val(x) {}
    ListNode() : next(nullptr), val(0) {}
};

class setLinked {
    public:
    ListNode *head;

    setLinked() {
        head = new ListNode();
    }
    ~setLinked() {
        ListNode *curr = head;
        while (curr != nullptr) {
            ListNode *next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void insert(int val) {
        ListNode *prev = find_last(val);

        // if exists
        if (prev->next != nullptr && prev->next->val == val)
            return;

        prev->next = new ListNode(val, prev->next);
    }

    void remove(int val) {
        ListNode *prev = find_last(val);
        if (prev->next != nullptr && prev->next->val == val) {
            ListNode *toDelete = prev->next;
            prev->next = toDelete->next;
            delete toDelete;
        }
    }

    bool contains(int val) const {
        ListNode *curr = find_last(val);
        return curr->next != nullptr && curr->next->val == val;
    }

    static setLinked unionSets(const setLinked &A, const setLinked &B) {
        setLinked answer;
        ListNode *first = A.head->next;
        ListNode *second = B.head->next;
        ListNode *curr = answer.head;
        while (first && second) {
            if (first->val < second->val) {
                curr->next = new ListNode(first->val);
                first = first->next;
            }
            else if (first->val > second->val) {
                curr->next = new ListNode(second->val);
                second = second->next;
            }
            else {
                curr->next = new ListNode(first->val);
                first = first->next;
                second = second->next;
            }
            curr = curr->next;
        }

        while (first) {
            curr->next = new ListNode(first->val);
            first = first->next;
            curr = curr->next;
        }

        while (second) {
            curr->next = new ListNode(second->val);
            second = second->next;
            curr = curr->next;
        }

        return answer;
    }

    static setLinked intersectionSets(const setLinked &A, const setLinked &B) {
        setLinked answer;
        ListNode *first = A.head->next;
        ListNode *second = B.head->next;
        ListNode *curr = answer.head;
        // 1->2->3
        // 2->3->4
        while (first && second) {
            if (first->val < second->val) {
                first = first->next;
            }
            else if (first->val > second->val) {
                second = second->next;
            }
            else {
                curr->next = new ListNode(first->val);
                first = first->next;
                second = second->next;
                curr = curr->next;
            }
        }
        return answer;
    }

    static setLinked differenceSets(const setLinked &A, const setLinked &B) {   }

    void print() const {
        ListNode *curr = head->next;
        while (curr != nullptr) {
            std::cout << curr->val << " -> ";
            curr = curr->next;
        }
        std::cout << "nullptr";
        std::cout << std::endl;
    }

private:

    // finding prev pointer of desired value
    ListNode* find_last(int val) const {
        ListNode *curr = head;
        while (curr->next != nullptr && curr->next->val < val) {
            curr = curr->next;
        }
        return curr;
    }
};

int main() {
    setLinked set;
    set.insert(1);
    set.insert(2);
    set.insert(3);
    set.insert(4);
    set.insert(5);
    set.print();
    set.remove(2);
    set.print();
    set.insert(2);
    set.print();
    set.insert(6);
    set.insert(-1);
    std::cout << set.contains(5) << std::endl;
    std::cout << set.contains(10) << std::endl;

    setLinked A, B;
    A.insert(1);
    A.insert(2);
    A.insert(3);
    B.insert(2);
    B.insert(3);
    B.insert(4);

    A.insert(6);

    A.print();
    B.print();
    setLinked C = setLinked::unionSets(A, B);
    C.print();

    setLinked D = setLinked::intersectionSets(A, B);
    D.print();

}