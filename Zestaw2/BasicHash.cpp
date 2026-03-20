#include <iostream>
#include <string>

class SetRange {
private:
    bool* arr;
    int n, m;

    int hash(int x) const {
        return x - n;
    }

public:
    SetRange(int n, int m): n(n), m(m) {
        int size = m - n + 1;
        arr = new bool[size]{false};
    }
    ~SetRange() { delete[] arr; }

    void insert(int x) {
        if (x >= n && x <= m) arr[hash(x)] = true;
    }

    void remove(int x) {
        if (x >= n && x <= m) arr[hash(x)] = false;
    }

    bool contains(int x) const {
        return x >= n && x <= m && arr[hash(x)];
    }
};

class SetEvenRange {
private:
    bool* arr;
    int n, m;

    int hash(int x) const {
        return (x - n) / 2;
    }

public:
    SetEvenRange(int n, int m): n(n), m(m) {
        int size = (m - n) / 2 + 1;
        arr = new bool[size]{false};
    }
    ~SetEvenRange() { delete[] arr; }

    void insert(int x) {
        if (x >= n && x <= m) arr[hash(x)] = true;
    }

    void remove(int x) {
        if (x >= n && x <= m) arr[hash(x)] = false;
    }

    bool contains(int x) const {
        return x >= n && x <= m && (x - n) % 2 == 0 && arr[hash(x)];
    }
};

class SetChar {
private:
    char* arr;

    int hash(char x) const {
        return x - 'a';
    }

public:
    SetChar() {
        arr = new char[26]{false};
    }
    ~SetChar() { delete[] arr; }

    void insert(char x) {
        if (x >= 'a' && x <= 'z') arr[hash(x)] = true;
    }

    void remove(char x) {
        if (x >= 'a' && x <= 'z') arr[hash(x)] = false;
    }

    bool contains(char x) const {
        return x >= 'a' && x <= 'z' && arr[hash(x)];
    }
};

class SetTwoChars {
private:
    bool* arr;

    int hash(const std::string& x) const {
        return x[0] - 'a' + (x[1] - 'a') * 26;
    }

    bool isValid(const std::string& x) const {
        return x.size() == 2 && x[0] >= 'a' && x[0] <= 'z' && x[1] >= 'a' && x[1] <= 'z';
    }

public:
    SetTwoChars() {
        arr = new bool[26*26]{false};
    }
    ~SetTwoChars() { delete[] arr; }

    void insert(const std::string& x) {
        if (isValid(x)) arr[hash(x)] = true;
    }

    void remove(const std::string& x) {
        if (isValid(x)) arr[hash(x)] = false;
    }

    bool contains(const std::string& x) const {
        return isValid(x) && arr[hash(x)];
    }
};

int main() {
    std::cout << "--- Test 1: Zakres [-5, 5] ---\n";
    SetRange sr(-5, 5);
    sr.insert(-3);
    sr.insert(4);
    std::cout << "Czy ma -3? " << (sr.contains(-3) ? "Ta" : "Nie") << "\n";
    std::cout << "Czy ma 0?  " << (sr.contains(0) ? "Ta" : "Nie") << "\n";

    std::cout << "\n--- Test 2: Skok co 2 [10, 20] ---\n";
    SetEvenRange ser(10, 20);
    ser.insert(12);
    ser.insert(18);
    std::cout << "Czy ma 12? " << (ser.contains(12) ? "Ta" : "Nie") << "\n";
    std::cout << "Czy ma 13?" << (ser.contains(13) ? "Ta" : "Nie") << "\n";

    std::cout << "\n--- Test 3: Litery [a-z] ---\n";
    SetChar sc;
    sc.insert('k');
    sc.insert('z');
    sc.remove('k');
    std::cout << "Czy ma 'k' po usunieciu? " << (sc.contains('k') ? "Ta" : "Nie") << "\n";
    std::cout << "Czy ma 'z'? " << (sc.contains('z') ? "Ta" : "Nie") << "\n";

    std::cout << "\n--- Test 4: Stringi [aa-zz] ---\n";
    SetTwoChars ss;
    ss.insert("xd");
    ss.insert("zz");
    std::cout << "Czy ma 'xd'? " << (ss.contains("xd") ? "Ta" : "Nie") << "\n";
    std::cout << "Czy ma 'ab'? " << (ss.contains("ab") ? "Ta" : "Nie") << "\n";
}