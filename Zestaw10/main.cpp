#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>

using namespace std;

// Sprawdza, czy graf jest nieskierowany
bool is_symmetric(const vector<vector<int>>& adj) {
    int n = adj.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (adj[i][j] != adj[j][i]) return false;
        }
    }
    return true;
}

// DFS dla nieskierowanych
bool dfs_undir(int v, int parent, const vector<vector<int>>& adj, vector<bool>& visited) {
    visited[v] = true;
    for (size_t u = 0; u < adj.size(); ++u) {
        if (adj[v][u]) {
            if (!visited[u]) {
                if (dfs_undir(u, v, adj, visited)) return true;
            } else if (u != parent) {
                return true;
            }
        }
    }
    return false;
}

// DFS dla skierowanych
bool dfs_dir(int v, const vector<vector<int>>& adj, vector<bool>& visited, vector<bool>& rec_stack) {
    visited[v] = true;
    rec_stack[v] = true;
    for (size_t u = 0; u < adj.size(); ++u) {
        if (adj[v][u]) {
            if (!visited[u]) {
                if (dfs_dir(u, adj, visited, rec_stack)) return true;
            } else if (rec_stack[u]) {
                return true;
            }
        }
    }
    rec_stack[v] = false;
    return false;
}

// sprawdzanie cykli w zależności od typu grafu
bool check_cycle(const vector<vector<int>>& adj) {
    int n = adj.size();
    if (n == 0) return false;
    vector<bool> visited(n, false);

    if (is_symmetric(adj)) {
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                if (dfs_undir(i, -1, adj, visited)) return true;
            }
        }
    } else {
        vector<bool> rec_stack(n, false);
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                if (dfs_dir(i, adj, visited, rec_stack)) return true;
            }
        }
    }
    return false;
}

// BFS do sprawdzania spójności
int bfs(int start, const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(start);
    visited[start] = true;
    int count = 1;

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u = 0; u < n; ++u) {
            if (adj[v][u] && !visited[u]) {
                visited[u] = true;
                q.push(u);
                count++;
            }
        }
    }
    return count;
}

// Sprawdza spójność
bool check_connectivity(const vector<vector<int>>& adj) {
    int n = adj.size();
    if (n == 0) return false;

    if (is_symmetric(adj)) {
        return bfs(0, adj) == n;
    } else {
        // Silna spójność
        if (bfs(0, adj) != n) return false;

        vector<vector<int>> rev_adj(n, vector<int>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                rev_adj[j][i] = adj[i][j];
            }
        }
        return bfs(0, rev_adj) == n;
    }
}

// Wczytywanie macierzy z pliku
vector<vector<int>> load_graph(const string& filename) {
    vector<vector<int>> adj;
    ifstream file(filename);
    if (!file.is_open()) return adj;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<int> row;
        stringstream ss(line);
        int val;
        while (ss >> val) {
            row.push_back(val);
        }
        if (!row.empty()) adj.push_back(row);
    }
    return adj;
}

int main() {
    vector<string> files = {"../h1", "../h2", "../h3", "../g1", "../g2", "../g3"};

    cout << "--- TESTY ---" << endl;
    for (const string& file : files) {
        vector<vector<int>> matrix = load_graph(file);

        if (matrix.empty()) {
            cout << "[" << file << "] -> Pliku nie znaleziono." << endl;
            continue;
        }

        string typ = is_symmetric(matrix) ? "Nieskierowany" : "Skierowany";
        string cykl = check_cycle(matrix) ? "TAK" : "NIE";
        string spojny = check_connectivity(matrix) ? "TAK" : "NIE";

        cout << "[" << file << "] (" << typ << ") -> Cykle: " << cykl << " | Spojny: " << spojny << endl;
    }

    return 0;
}