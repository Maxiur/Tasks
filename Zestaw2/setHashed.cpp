#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <vector>
#include <random>
#include <iomanip>

struct ListNode {
    ListNode *next;
    int val;

    ListNode(const int x, ListNode *next) : next(next), val(x) {}
    ListNode(const int x) : next(nullptr), val(x) {}
    ListNode() : next(nullptr), val(0) {}
    ~ListNode() = default;
};

class SetHashed {
private:
    int N;
    ListNode** heads;

    int hash(int x) const {
        return std::abs(x) % N;
    }

public:
    SetHashed(int n) : N(n) {
        heads = new ListNode*[N];
        for (int i = 0; i < N; ++i) heads[i] = new ListNode();
    }

    ~SetHashed() {
        if (!heads) return;
        for (int i = 0; i < N; ++i) {
            ListNode* curr = heads[i];
            while (curr) {
                ListNode* toDelete = curr;
                curr = curr->next;
                delete toDelete;
            }
        }
        delete[] heads;
    }

    // Deep Copy
    SetHashed(const SetHashed& other) : SetHashed(other.N) {
        for (int i = 0; i < N; ++i) {
            ListNode* otherCurr = other.heads[i]->next;
            ListNode* curr = heads[i];
            while (otherCurr) {
                curr->next = new ListNode(otherCurr->val);
                curr = curr->next;
                otherCurr = otherCurr->next;
            }
        }
    }

    // Przypisania kopiujący
    SetHashed& operator=(const SetHashed& other) {
        if (this != &other) {
            SetHashed temp(other);
            std::swap(heads, temp.heads);
            std::swap(N, temp.N);
        }
        return *this;
    }

    // Przenoszący (move)
    SetHashed(SetHashed&& other) noexcept : N(other.N), heads(other.heads) {
        other.N = 0;
        other.heads = nullptr;
    }

    // Przypisania przenoszący
    SetHashed& operator=(SetHashed&& other) noexcept {
        if (this != &other) {
            std::swap(heads, other.heads);
            std::swap(N, other.N);
        }
        return *this;
    }

    bool contains(int x) const {
        ListNode* curr = heads[hash(x)]->next;
        while (curr && curr->val <= x) {
            if (curr->val == x) return true;
            curr = curr->next;
        }
        return false;
    }

    void insert(int x) {
        ListNode* curr = heads[hash(x)];

        while (curr->next && curr->next->val < x) {
            curr = curr->next;
        }

        if (curr->next && curr->next->val == x) return;

        // we insert between Nodes
        curr->next = new ListNode(x, curr->next);
    }

    void remove(int x) {
        ListNode* curr = heads[hash(x)];

        while (curr->next && curr->next->val < x) {
            curr = curr->next;
        }

        if (curr->next && curr->next->val == x) {
            ListNode* toDelete = curr->next;
            curr->next = toDelete->next;
            delete toDelete;
        }
    }

    void print() const {
        std::cout << " = { ";
        for (int i = 0; i < N; ++i) {
            ListNode* curr = heads[i]->next;
            while (curr) {
                std::cout << curr->val << " ";
                curr = curr->next;
            }
            if (i != N - 1) {
                std::cout << "} " <<std::endl;
                std::cout << "{ ";
            }
        }
        std::cout << "}\n";
}

    SetHashed Union(const SetHashed& other) const {
        if (this->N != other.N) {
            throw std::invalid_argument("Zbiory muszą miec te sama liczbe kubelkow");
        }

        SetHashed answer(N);
        for (int i = 0; i < N; ++i) {
            ListNode* first = this->heads[i]->next;
            ListNode* second = other.heads[i]->next;
            ListNode* curr = answer.heads[i];

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
        }
        return answer;
    }

    SetHashed Intersection(const SetHashed& other) const {
        if (this->N != other.N) {
            throw std::invalid_argument("Zbiory muszą miec te sama liczbe kubelkow");
        }

        SetHashed answer(this->N);

        for (int i = 0; i < N; ++i) {
            ListNode* first = this->heads[i]->next;
            ListNode* second = other.heads[i]->next;
            ListNode* curr = answer.heads[i];

            while (first && second) {
                if (first->val == second->val) {
                    curr->next = new ListNode(first->val);
                    curr = curr->next;
                    first = first->next;
                    second = second->next;
                }
                else if (first->val < second->val) {
                    first = first->next;
                }
                else {
                    second = second->next;
                }
            }
        }
        return answer;
    }

    SetHashed Difference(const SetHashed& other) const {
        if (this->N != other.N) {
            throw std::invalid_argument("Zbiory muszą miec te sama liczbe kubelkow");
        }

        SetHashed answer(this->N);

        for (int i = 0; i < N; ++i) {
            ListNode* first = this->heads[i]->next;
            ListNode* second = other.heads[i]->next;
            ListNode* curr = answer.heads[i];

            while (first && second) {
                if (first->val < second->val) {
                    curr->next = new ListNode(first->val);
                    curr = curr->next;
                    first = first->next;
                }
                else if (first->val > second->val) {
                    second = second->next;
                }
                else {
                    first = first->next;
                    second = second->next;
                }
            }
            while (first) {
                curr->next = new ListNode(first->val);
                curr = curr->next;
                first = first->next;
            }
        }
        return answer;
    }

    bool operator==(const SetHashed& other) const {
        if (this->N != other.N) {
            return false;
        }

        for (int i = 0; i < N; ++i) {
            ListNode* first = this->heads[i]->next;
            ListNode* second = other.heads[i]->next;

            while (first && second) {
                if (first->val != second->val) return false;
                first = first->next;
                second = second->next;
            }
            if (first || second) return false;
        }
        return true;
    }
};

void run_benchmarks() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 2000000);

    const int N_buckets = 100;

    std::cout << std::left << std::setw(10) << "n"
              << std::setw(12) << "Insert"
              << std::setw(12) << "Contains"
              << std::setw(12) << "Remove"
              << std::setw(12) << "Union"
              << std::setw(12) << "Inter"
              << std::setw(12) << "Diff" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (int n = 1000; n <= 30000; n += 2000) {
        SetHashed A(N_buckets);
        SetHashed B(N_buckets);

        std::vector<int> values;
        for(int i = 0; i < n; ++i) values.push_back(dis(gen));

        std::vector<int> other_values;
        for(int i = 0; i < n; ++i) other_values.push_back(dis(gen));

        // 1. Benchmark INSERT
        auto start = std::chrono::high_resolution_clock::now();
        for(int val : values) A.insert(val);
        auto end = std::chrono::high_resolution_clock::now();
        auto d_ins = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        for(int val : other_values) B.insert(val);

        // 2. Benchmark CONTAINS
        start = std::chrono::high_resolution_clock::now();
        for(int val : values) A.contains(val);
        end = std::chrono::high_resolution_clock::now();
        auto d_cont = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // 3. Benchmark UNION
        start = std::chrono::high_resolution_clock::now();
        SetHashed res_u = A.Union(B);
        end = std::chrono::high_resolution_clock::now();
        auto d_union = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // 4. Benchmark INTERSECTION
        start = std::chrono::high_resolution_clock::now();
        SetHashed res_i = A.Intersection(B);
        end = std::chrono::high_resolution_clock::now();
        auto d_inter = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // 5. Benchmark DIFFERENCE
        start = std::chrono::high_resolution_clock::now();
        SetHashed res_diff = A.Difference(B);
        end = std::chrono::high_resolution_clock::now();
        auto d_diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // 6. Benchmark REMOVE
        start = std::chrono::high_resolution_clock::now();
        for(int val : values) A.remove(val);
        end = std::chrono::high_resolution_clock::now();
        auto d_rem = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Wypisanie wyników w jednej linii (ułatwia import do Pythona/Excela)
        std::cout << std::left << std::setw(10) << n
                  << std::setw(12) << d_ins
                  << std::setw(12) << d_cont
                  << std::setw(12) << d_rem
                  << std::setw(12) << d_union
                  << std::setw(12) << d_inter
                  << std::setw(12) << d_diff << std::endl;
    }
}

int main() {
    // run_benchmarks();

    SetHashed A(10);
    SetHashed B(10);

    for (int i = 0; i < 101; i += 5) {
        A.insert(i);
        B.insert(i);
    }
    A.print();
    A.Intersection(B).print();
    A.Union(B).print();

    std::cout << (A.contains(10) ? "Tak " : "Nie ") << std::endl;
    std::cout << (A.contains(11) ? "Tak " : "Nie ") << std::endl;

    std::cout << "-------------------------------" << std::endl;

    SetHashed C(10);
    SetHashed D(10);
    for (int i = 0; i < 101; i += 5) {
        C.insert(i);
        D.insert(i + 1);
    }

    C.print();
    D.print();

    C.Intersection(D).print();
    C.Union(D).print();
    C.Difference(D).print();

    return 0;
}