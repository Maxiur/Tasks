#include <chrono>
#include <iostream>

struct ListNode {
    ListNode *next;
    int val;

    ListNode(const int x, ListNode *next) : next(next), val(x) {}
    ListNode(const int x) : next(nullptr), val(x) {}
    ListNode() : next(nullptr), val(0) {}
    ~ListNode() = default;
};

class setLinked {
public:
    ListNode *head;

    setLinked() {
        head = new ListNode();
    }

    setLinked(const setLinked &other) {
        head = new ListNode();
        ListNode* curr = head;
        ListNode* otherCurr = other.head->next;
        while (otherCurr != nullptr) {
            curr->next = new ListNode(otherCurr->val);
            curr = curr->next;
            otherCurr = otherCurr->next;
        }
    }

    setLinked& operator=(const setLinked &other) {
        if (this == &other) return *this;

        ListNode* curr = head->next;
        while (curr) {
            ListNode* next = curr->next;
            delete curr;
            curr = next;
        }
        head->next = nullptr;

        curr = head;
        ListNode* otherCurr = other.head->next;
        while (otherCurr) {
            curr->next = new ListNode(otherCurr->val);
            curr = curr->next;
            otherCurr = otherCurr->next;
        }
        return *this;
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

    static setLinked differenceSets(const setLinked &A, const setLinked &B) {
        setLinked answer;
        ListNode *first = A.head->next;
        ListNode *second = B.head->next;
        ListNode *curr = answer.head;

        while (first && second) {
            if (first->val < second->val) {
                curr->next = new ListNode(first->val);
                first = first->next;
                curr = curr->next;
            }
            else if (first->val == second->val) {
                first = first->next;
                second = second->next;
            }
            else {
                second = second->next;
            }
        }
        while (first) {
            curr->next = new ListNode(first->val);
            first = first->next;
            curr = curr->next;
        }
        return answer;
    }

    static bool areEqual(const setLinked &A, const setLinked &B) {
        ListNode* first = A.head->next;
        ListNode* second = B.head->next;

        while (first && second) {
            if (first->val != second->val) return false;
            first = first->next;
            second = second->next;
        }
        if (first || second) return false;
        return true;
    }

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

void run_benchmark() {
    // Dodana kolumna AreEqual_us
    std::cout << "N,Union_us,Intersection_us,Difference_us,AreEqual_us\n";

    std::vector<int> sizes = {1000, 2000, 4000, 6000, 8000, 10000, 15000, 20000, 40000, 60000, 100000, 2000000};

    for (int N : sizes) {
        setLinked A, B;

        for (int i = N; i > 0; --i) {
            A.insert(i * 2);
            B.insert(i * 2 - 1);
        }

        // UNION
        auto start = std::chrono::high_resolution_clock::now();
        setLinked C = setLinked::unionSets(A, B);
        auto end = std::chrono::high_resolution_clock::now();
        auto time_union = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // INTERSECTION
        start = std::chrono::high_resolution_clock::now();
        setLinked D = setLinked::intersectionSets(A, B);
        end = std::chrono::high_resolution_clock::now();
        auto time_intersect = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // DIFFERENCE
        start = std::chrono::high_resolution_clock::now();
        setLinked E = setLinked::differenceSets(A, B);
        end = std::chrono::high_resolution_clock::now();
        auto time_diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // ARE EQUAL
        setLinked A_copy = A;
        start = std::chrono::high_resolution_clock::now();
        bool eq = setLinked::areEqual(A, A_copy);
        end = std::chrono::high_resolution_clock::now();
        auto time_eq = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << N << "," << time_union << "," << time_intersect << "," << time_diff << "," << time_eq << "\n";
    }
}

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

    setLinked E = setLinked::differenceSets(A, B);
    E.print();

    // run_benchmark();

}