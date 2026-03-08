#include <chrono>
#include <iostream>
#include <vector>

class SetSimple {
public:
    std::vector<bool> elements;
    int n;

    SetSimple(int size) : n(size), elements(size, false) {}

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
    // Różne rozmiary N do badania złożoności
    std::vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000};

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "N\t\tSuma [s]\tCz. wspolna [s]\tRoznica [s]\n";
    std::cout << "------------------------------------------------------------\n";

    for (int n : sizes) {
        SetSimple A(n), B(n);
        for(int i=0; i<n; i+=2) A.insert(i);
        for(int i=0; i<n; i+=2) B.insert(i);

        auto s_start = std::chrono::high_resolution_clock::now();
        SetSimple resU = SetSimple::unionSets(A, B);
        auto s_end = std::chrono::high_resolution_clock::now();

        auto i_start = std::chrono::high_resolution_clock::now();
        SetSimple resI = SetSimple::intersectionSets(A, B);
        auto i_end = std::chrono::high_resolution_clock::now();

        auto d_start = std::chrono::high_resolution_clock::now();
        SetSimple resD = SetSimple::differenceSets(A, B);
        auto d_end = std::chrono::high_resolution_clock::now();
        
        auto e_start = std::chrono::high_resolution_clock::now();
        SetSimple resE = SetSimple::areEqual(A, B);
        auto e_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> tU = s_end - s_start;
        std::chrono::duration<double> tI = i_end - i_start;
        std::chrono::duration<double> tD = d_end - d_start;
        std::chrono::duration<double> tE = e_end - e_start;

        std::cout << n << "\t" << tU.count() << "\t" << tI.count() << "\t" << tD.count() << "\t " << tE.count() << "\n";
    }
}

int main() {
    run_benchmarks();
    return 0;
}