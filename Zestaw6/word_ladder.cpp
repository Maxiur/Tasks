#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

// Magiczny rozcinacz UTF-8.
vector<string> split_utf8(const string& str) {
    vector<string> chars;
    for (size_t i = 0; i < str.length(); ) {
        int cplen = 1;
        if ((str[i] & 0xE0) == 0xC0) cplen = 2; // Polski znak (2 bajty)
        else if ((str[i] & 0xF0) == 0xE0) cplen = 3;
        else if ((str[i] & 0xF8) == 0xF0) cplen = 4;
        chars.push_back(str.substr(i, cplen));
        i += cplen;
    }
    return chars;
}

int main() {
    string start_word = "matma";
    string end_word = "radom";

    int word_len = split_utf8(start_word).size();

    string dict_filename = "../slowa/slowa_" + to_string(word_len) + ".txt";

    cout << "Ładuje gotowy plik: " << dict_filename << "..." << endl;

    unordered_set<string> words;
    ifstream file(dict_filename);

    if (!file.is_open()) {
        cout << "Błąd: Nie ma pliku " << dict_filename << ". Odpaliłeś najpierw szatkownicę w Pythonie?" << endl;
        return 1;
    }

    string line;

    while (file >> line) {
        words.insert(line);
    }
    file.close();

    if (words.find(end_word) == words.end()) {
        cout << "Słowo końcowe nie istnieje w słowniku." << endl;
        return 1;
    }

    vector<string> alphabet = split_utf8("aąbcćdeęfghijklłmnńoópqrsśtuvwxyzźż");

    queue<string> q;
    unordered_map<string, string> parent;

    q.push(start_word);
    parent[start_word] = ""; // start nie ma rodzica

    bool found = false;
    cout << "Szukam ścieżki BFS-em..." << endl;

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == end_word) {
            found = true;
            break;
        }

        // Tniemy aktualne słowo na 4 litery
        vector<string> current_chars = split_utf8(current);

        // Tworzymy mutacje
        for (size_t i = 0; i < current_chars.size(); ++i) {
            string original_char = current_chars[i];

            for (const string& letter : alphabet) {
                if (letter == original_char) continue;

                current_chars[i] = letter; // Podmiana literki

                // Sklejamy z powrotem w stringa
                string next_word = "";
                for (const string& c : current_chars) next_word += c;

                // Jeśli mutant jest w słowniku i go nie widzieliśmy -> do kolejki
                if (words.find(next_word) != words.end() && parent.find(next_word) == parent.end()) {
                    parent[next_word] = current;
                    q.push(next_word);
                }
            }
            current_chars[i] = original_char; // Cofamy zmianę przed następną pętlą
        }
    }

    // Wypisywanie wyników
    if (found) {
        vector<string> path;
        string curr = end_word;
        while (curr != "") {
            path.push_back(curr);
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());

        cout << "\nMamy to! Trasa:" << endl;
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i] << (i < path.size() - 1 ? " -> " : "");
        }
        cout << "\nLiczba operacji: " << path.size() - 1 << endl;
    } else {
        cout << "Nie da się z tego wyjść. Brak ścieżki." << endl;
    }

    return 0;
}