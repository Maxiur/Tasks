#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

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

int main() {
    SetStrings mySet(3);

    mySet.insert("ABC");
    mySet.insert("DEF");
    mySet.insert("XYZ");
    mySet.insert("ABC"); // Duplikat

    std::cout << "Po wstawieniu: ";
    mySet.print();

    std::cout << "Czy zawiera 'DEF'? " << (mySet.contains("DEF") ? "Tak" : "Nie") << std::endl;
    std::cout << "Czy zawiera 'GHI'? " << (mySet.contains("GHI") ? "Tak" : "Nie") << std::endl;

    mySet.remove("DEF");
    std::cout << "Po usunieciu 'DEF': ";
    mySet.print();
}