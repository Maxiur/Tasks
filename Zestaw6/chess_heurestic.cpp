#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <fstream>
#include <algorithm>

constexpr int N = 8;

// legal moves
int cx[8] = {1, 1, 2, 2, -1, -1, -2, -2};
int cy[8] = {2, -2, 1, -1, 2, -2, 1, -1};

bool limit(int x, int y) {
    return ((x >= 0 && y >= 0) && (x < N && y < N));
}

// Funkcja heurystyczna: liczy ile legalnych i wolnych pól jest dostępnych z (x, y)
int getDegree(const std::vector<std::vector<int>>& graph, int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; ++i) {
        int nx = x + cx[i];
        int ny = y + cy[i];
        if (limit(nx, ny) && graph[nx][ny] == 0) {
            count++;
        }
    }
    return count;
}

bool backtracking(std::vector<std::vector<int>>& graph, std::vector<std::pair<int, int>>& moves, int x, int y) {
    if (moves.size() == N * N) return true;

    // Zbieramy wszystkie legalne ruchy z tego pola
    std::vector<std::pair<int, int>> next_moves;
    for (int i = 0; i < 8; i++) {
        int nx = x + cx[i];
        int ny = y + cy[i];

        if (limit(nx, ny) && graph[nx][ny] == 0) {
            next_moves.emplace_back(nx, ny);
        }
    }

    // Sortujemy zebrane ruchy rosnąco po liczbie ucieczek
    std::sort(next_moves.begin(), next_moves.end(), [&](std::pair<int, int> a, std::pair<int, int> b) {
        return getDegree(graph, a.first, a.second) < getDegree(graph, b.first, b.second);
    });

    // koń wybiera najtrudniejsze pola na początku
    for (auto next_move : next_moves) {
        int di = next_move.first;
        int dj = next_move.second;

        moves.emplace_back(di, dj);
        graph[di][dj] = 1;

        if (backtracking(graph, moves, di, dj))
            return true;

        moves.pop_back();
        graph[di][dj] = 0;
    }

    return false;
}

int main() {
    std::vector<std::vector<int>> graph(N, std::vector<int>(N, 0));
    std::vector<std::pair<int, int>> moves;

    int start_x = 0;
    int start_y = 0;

    if (limit(start_x, start_y)) {
        graph[start_x][start_y] = 1;
        moves.emplace_back(start_x, start_y);

        if (backtracking(graph, moves, start_x, start_y)) {
            std::cout << "Trasa znaleziona! Generuje wykres..." << std::endl;

            std::ofstream file("../ruchy.txt");
            for (auto move : moves) {
                file << move.first << " " << move.second << "\n";
            }
            file.close();

            system("/usr/local/bin/python3 ../wykres.py");
        }
        else {
            std::cout << "Nie da się znaleźć takiej trasy!" << std::endl;
        }
    }
    else {
        std::cout << "Podana pozycja startowa jest poza granicami szachownicy!" << std::endl;
    }

    return 0;
}
