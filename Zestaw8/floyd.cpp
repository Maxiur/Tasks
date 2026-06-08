#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

const int INF = 1e7;

struct Edge {
    std::string u, v;
    int wage;
};

int main() {
    std::ifstream file("../czasy");
    if (!file.is_open()) {
        std::cerr <<  "Nie udalo sie otworzyc pliku czasy.txt!\n";
        return 1;
    }

    std::map<std::string, int> city_to_index;
    std::vector<std::string> index_to_city;

    std::vector<Edge> edges;

    std::string u, v;
    int w;
    int city_counter = 0;

    // Dynamiczne wczytywanie danych linia po linii
    while (file >> u >> v >> w) {
        edges.push_back({u, v, w});

        // Jeśli miasta nie ma jeszcze w mapie, dodajemy je
        if (city_to_index.find(u) == city_to_index.end()) {
            city_to_index[u] = city_counter++;
            index_to_city.push_back(u);
        }
        if (city_to_index.find(v) == city_to_index.end()) {
            city_to_index[v] = city_counter++;
            index_to_city.push_back(v);
        }
    }
    file.close();

    int n = city_counter;

    // Inicjalizacja macierzy odległości
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
    }

    // Wypełnienie macierzy krawędziami z pliku
    for (const auto& edge : edges) {
        int i = city_to_index[edge.u];
        int j = city_to_index[edge.v];
        dist[i][j] = std::min(dist[i][j], edge.wage);
        dist[j][i] = std::min(dist[j][i], edge.wage); // Graf nieskierowany
    }

    // Floyd-Warshall
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Formatowanie i wyświetlanie wyników
    std::cout << std::left << std::setw(22) << "Miasto";
    for (int i = 0; i < n; ++i) {
        std::cout << std::right << std::setw(6) << i;
    }
    std::cout << "\n" << std::string(22 + n * 6, '-') << "\n";

    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(22) << index_to_city[i];
        for (int j = 0; j < n; ++j) {
            if (dist[i][j] == INF) {
                std::cout << std::right << std::setw(6) << "INF";
            } else {
                std::cout << std::right << std::setw(6) << dist[i][j];
            }
        }
        std::cout << "\n";
    }

    std::cout << "\nLEGENDA INDEKSOW:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << i << ": " << index_to_city[i] << "\n";
    }

    return 0;
}
