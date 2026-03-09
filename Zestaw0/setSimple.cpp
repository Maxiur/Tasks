#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

class SetSimple {
public:
    std::vector<bool> elements;
    int n;

    SetSimple(int size) : n(size), elements(size, false) {}
    ~SetSimple() = default;

    void insert(int x) { if (x >= 0 && x < n) elements[x] = true; }
    void remove(int x) { if (x >= 0 && x < n) elements[x] = false; }
    bool contains(int x) { return x >= 0 && x < n && elements[x]; }

    static SetSimple unionSets(const SetSimple& A, const SetSimple& B) {
        SetSimple answer(A.n);
        for (int i = 0; i < A.n; ++i) {
            answer.elements[i] = A.elements[i] || B.elements[i];
        }
        return answer;
    }

    static SetSimple intersectionSets(const SetSimple& A, const SetSimple& B) {
        SetSimple answer(A.n);
        for (int i = 0; i < A.n; ++i) {
            answer.elements[i] = A.elements[i] && B.elements[i];
        }
        return answer;
    }

    static SetSimple differenceSets(const SetSimple& A, const SetSimple& B) {
        SetSimple answer(A.n);
        for (int i = 0; i < A.n; ++i) {
            answer.elements[i] = A.elements[i] && !B.elements[i];
        }
        return answer;
    }

    static bool areEqual(const SetSimple& A, const SetSimple& B) {
        for (int i = 0; i < A.n; ++i) {
            if (A.elements[i] != B.elements[i]) return false;
        }
        return true;
    }

    void print() const {
        std::cout << "{ ";
        for (int i = 0; i < n; ++i) {
            if (elements[i]) std::cout << i << " ";
        }
        std::cout << "}" << std::endl;
    }

};

void run_benchmarks() {
    std::vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000};
    const int REPEATS = 10; // Ile razy powtarzamy dla uśrednienia

    for (int n : sizes) {
        double sumU = 0, sumI = 0, sumD = 0;

        SetSimple A(n), B(n);
        for (int i = 0; i < n; i += 2) {
            A.insert(i);
            B.insert(i);
        }

        for (int r = 0; r < REPEATS; ++r) {

            auto s_start = std::chrono::high_resolution_clock::now();
            auto resU = SetSimple::unionSets(A, B);
            sumU += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - s_start).count();

            auto i_start = std::chrono::high_resolution_clock::now();
            auto resI = SetSimple::intersectionSets(A, B);
            sumI += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - i_start).count();

            auto d_start = std::chrono::high_resolution_clock::now();
            auto resD = SetSimple::differenceSets(A, B);
            sumD += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - d_start).count();
        }

        std::cout << n << "," << sumU/REPEATS << "," << sumI/REPEATS << "," << sumD/REPEATS << "\n";
    }
}

int main() {
    run_benchmarks();

    SetSimple A(20);
    SetSimple B(20);

    A.insert(1);
    A.insert(5);
    A.insert(6);
    A.insert(7);
    A.insert(10);

    B.insert(1);
    B.insert(10);
    B.insert(11);
    B.insert(12);

    SetSimple C = SetSimple::unionSets(A, B);
    C.print();

    SetSimple D = SetSimple::intersectionSets(A, B);
    D.print();

    SetSimple E = SetSimple::differenceSets(A, B);
    E.print();

    return 0;
}