#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

class SetStrings {
private:
    std::vector<std::string> elements;
    int k;

public:
    SetStrings(int k): k(k) {}


    // O(n)
    [[nodiscard]] bool contains(const std::string& x) const {
        return std::ranges::any_of(elements, [&x](const std::string& s) {
            return s == x;
        });
    }

    void insert(const std::string &x) {
        if (x.length() != static_cast<size_t>(k)) return;
        if (!contains(x)) {
            elements.emplace_back(x);
        }
    }

    void remove(const std::string& x) {
        for (size_t i = 0; i < elements.size(); i++) {
            if (elements[i] == x) {
                elements[i] = elements.back();
                elements.pop_back();
                return;
            }
        }
    }

    void print() const {
        std::cout << "{ ";
        for (const auto& s: elements) {
            std::cout << "\"" << s << "\" ";
        }
        std::cout << "}" << std::endl;
    }
};

void run_benchmark() {
    std::cout << "N,Insert_us,Contains_us,Remove_us\n";

    std::vector<int> sizes = {1000, 2000, 4000, 6000, 8000, 10000, 15000, 20000, 40000, 60000, 100000};
    const int k = 50; // Długość stringa
    const int BATCH = 1000;

    for (int N : sizes) {
        SetStrings mySet(k);

        for (int i = 0; i < N; ++i) {
            std::string s = std::to_string(i);
            s.append(k - s.length(), 'A');
            mySet.insert(s);
        }

        std::string not_found = "NOTINSET!!";

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < BATCH; ++i) {
            mySet.contains(not_found);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time_contains = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < BATCH; ++i) {
            mySet.remove(not_found);
        }
        end = std::chrono::high_resolution_clock::now();
        auto time_remove = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < BATCH; ++i) {
            std::string s = "IN" + std::to_string(i);
            s.append(k - s.length(), 'B');
            mySet.insert(s);
        }
        end = std::chrono::high_resolution_clock::now();
        auto time_insert = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << N << "," << time_insert << "," << time_contains << "," << time_remove << "\n";
    }
}

int main() {
    // SetStrings mySet(3);
    //
    // mySet.insert("ABC");
    // mySet.insert("DEF");
    // mySet.insert("XYZ");
    // mySet.insert("ABC"); // Duplikat
    //
    // std::cout << "Po wstawieniu: ";
    // mySet.print();
    //
    // std::cout << "Czy zawiera 'DEF'? " << (mySet.contains("DEF") ? "Tak" : "Nie") << std::endl;
    // std::cout << "Czy zawiera 'GHI'? " << (mySet.contains("GHI") ? "Tak" : "Nie") << std::endl;
    //
    // mySet.remove("DEF");
    // std::cout << "Po usunieciu 'DEF': ";
    // mySet.print();
}