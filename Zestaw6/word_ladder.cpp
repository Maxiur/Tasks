#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>

// UTF-8
std::vector<std::string> split_utf8(const std::string& str) {
    std::vector<std::string> chars;
    for (std::size_t i = 0; i < str.length(); ) {
        int cplen = 1;
        if ((str[i] & 0xE0) == 0xC0) cplen = 2;
        else if ((str[i] & 0xF0) == 0xE0) cplen = 3;
        else if ((str[i] & 0xF8) == 0xF0) cplen = 4;
        chars.push_back(str.substr(i, cplen));
        i += cplen;
    }
    return chars;
}

std::string join(const std::vector<std::string>& vec) {
    std::string res = "";
    for (const std::string& s : vec) res += s;
    return res;
}

int main() {
    std::string start_word = "matma";
    std::string end_word = "radom";

    int target_len = split_utf8(start_word).size();
    std::string dict_filename = "../slowa/slowa_" + std::to_string(target_len) + ".txt";

    std::cout << "Ladowanie slownika\n";

    std::ifstream file(dict_filename);
    if (!file.is_open()) {
        std::cout << "Błąd: Brak pliku " << dict_filename << "!\n";
        return 1;
    }


    // Klucz: maska (np. s*er), Wartość: lista pasujących słów
    std::unordered_map<std::string, std::vector<std::string>> patterns;
    std::unordered_set<std::string> valid_words;

    std::string line;
    while (file >> line) {
        std::vector<std::string> chars = split_utf8(line);
        // Bierzemy tylko słowa o tej samej długości
        if (chars.size() == target_len) {
            valid_words.insert(line);

            // Generujemy maski i wrzucamy do mapy
            for (std::size_t i = 0; i < chars.size(); ++i) {
                std::string original = chars[i];
                chars[i] = "*";
                patterns[join(chars)].push_back(line);
                chars[i] = original; // Cofamy zmianę
            }
        }
    }
    file.close();

    if (valid_words.find(end_word) == valid_words.end()) {
        std::cout << "Koniec! Słowa końcowego nie ma w słowniku!\n";
        return 1;
    }

    std::queue<std::string> q;
    std::unordered_map<std::string, std::string> parent; // Do odtworzenia trasy

    q.push(start_word);
    parent[start_word] = ""; // Start nie ma rodzica

    bool found = false;

    while (!q.empty()) {
        std::string curr = q.front();
        q.pop();

        if (curr == end_word) {
            found = true;
            break;
        }

        std::vector<std::string> chars = split_utf8(curr);

        // Sprawdzamy wszystkie możliwe maski dla obecnego słowa
        for (std::size_t i = 0; i < chars.size(); ++i) {
            std::string original = chars[i];
            chars[i] = "*";
            std::string pattern = join(chars);
            chars[i] = original; // Cofamy zmianę przed następną iteracją

            // Wyciągamy wszystkich pasujących do maski
            for (const std::string& neighbor : patterns[pattern]) {
                // Jeżeli go jeszcze nie odwiedziliśmy, to dodajemy do kolejki
                if (parent.find(neighbor) == parent.end()) {
                    parent[neighbor] = curr;
                    q.push(neighbor);
                }
            }
        }
    }

    // Wypisanie wyniku
    if (found) {
        std::vector<std::string> path;
        std::string curr = end_word;
        while (curr != "") {
            path.push_back(curr);
            curr = parent[curr];
        }
        std::reverse(path.begin(), path.end());

        for (std::size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i < path.size() - 1 ? " -> " : "");
        }
        std::cout << "\nLiczba operacji: " << path.size() - 1 << std::endl;
    } else {
        std::cout << "Nie da się z tego wyjść. Ślepy zaułek." << std::endl;
    }

    return 0;
}
