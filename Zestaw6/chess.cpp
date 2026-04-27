#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <fstream>

constexpr int N = 8;

// legal moves
int cx[8] = {1, 1, 2, 2, -1, -1, -2, -2};
int cy[8] = {2, -2, 1, -1, 2, -2, 1, -1};

bool limit(int x, int y) {
    return ((x >= 0 && y >= 0) && (x < N && y < N));
}

bool backtracking(std::vector<std::vector<int>>& graph, std::vector<std::pair<int, int>>& moves, int x, int y) {
    if (moves.size() == N * N) return true;

    for (int i = 0; i < 8; i++) {
        int di = x + cx[i];
        int dj = y + cy[i];

        if (limit(di, dj) && graph[di][dj] == 0) {
            moves.emplace_back(di, dj);
            graph[di][dj] = 1;
            if (backtracking(graph, moves, di, dj))
                return true;
            moves.pop_back();
            graph[di][dj] = 0;
        }
    }

    return false;
}

int main() {
    std::vector graph(N, std::vector<int>(N, 0));
    std::vector<std::pair<int, int>> moves;

    int start_x = 0;
    int start_y = 0;

    if (limit(start_x, start_y)) {
        graph[start_x][start_y] = 1;
        moves.emplace_back(start_x, start_y);
        if (backtracking(graph, moves, start_x, start_y)) {
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

}