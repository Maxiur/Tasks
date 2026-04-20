#include <iostream>
#include <unordered_map>
#include <vector>

template <typename V, typename E>
class Graph {
private:
    struct Edge {
        E value{};
    };

    std::vector<std::unordered_map<int, Edge>> adjList;
    std::vector<V> vertices;

public:
    Graph() = default;

    bool adjacent(int x, int y) const {
        if (x >= vertices.size() || y >= vertices.size()) return false;
        return adjList[x].count(y) > 0;
    }

    std::vector<int> neighbours(int x) const {
        if (x >= vertices.size()) throw std::out_of_range("Brak wierzcholka");
        std::vector<int> neighbours;

        for (const auto& pair : adjList[x]) {
            neighbours.push_back(pair.first);
        }
        return neighbours;
    }

    int addVertex(V val) {
        vertices.push_back(val);
        adjList.emplace_back();
        return vertices.size() - 1;
    }

    void removeVertex(int x) {
        if (x >= vertices.size()) return;

        for (auto& map : adjList) {
            map.erase(x);
        }

        adjList[x].clear();

        int lastIndex = vertices.size() - 1;

        if (x != lastIndex) {
            // przerzucamy na miejsce usuwanego
            vertices[x] = std::move(vertices[lastIndex]);
            adjList[x] = std::move(adjList[lastIndex]);

            for (auto& map : adjList) {
                if (map.count(lastIndex)) {
                    map[x] = std::move(map[lastIndex]);
                    map.erase(lastIndex);
                }
            }
        }

        vertices.pop_back();
        adjList.pop_back();
    }

    void addEdge(int x, int y) {
        if (x >= vertices.size() || y >= vertices.size()) return;
        adjList[x][y] = Edge{};
    }

    void removeEdge(int x, int y) {
        if (x >= vertices.size() || y >= vertices.size()) return;
        adjList[x].erase(y);
    }

    V getVertexValue(int x) const {
        return vertices[x];
    }

    void setVertexValue(int x, V v) {
        vertices.at(x) = v;
    }

    E getEdgeValue(int x, int y) const {
        if (!adjacent(x, y)) throw std::out_of_range("Brak krawedzi");
        return adjList.at(x).at(y).value;
    }

    void setEdgeValue(int x, int y, E v) {
        if (!adjacent(x, y)) throw std::out_of_range("Brak krawedzi");
        adjList.at(x).at(y).value = v;
    }
};

void benchmark() {
    std::cout << "Liczba_sasiadow(K),Czas_neighbours(ns)\n";

    // Skalujemy liczbę sąsiadów od 1000 do 10000
    for (int k = 1000; k <= 10000; k += 1000) {
        Graph<int, int> G;
        int v0 = G.addVertex(0);

        for (int i = 1; i <= k; ++i) {
            G.addVertex(i);
            G.addEdge(v0, i);
        }

        auto start = std::chrono::high_resolution_clock::now();

        // Zabezpieczamy przed wyrzuceniem przez optymalizator kompilatora
        volatile size_t dummy = G.neighbours(v0).size();

        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << k << "," << duration << "\n";
    }
}

int main() {
    std::cout << "--- TESTY  ---\n";
    Graph<std::string, double> myGraph;

    int v0 = myGraph.addVertex("Krakow"); // Index 0
    int v1 = myGraph.addVertex("Warszawa"); // Index 1
    int v2 = myGraph.addVertex("Poznan");   // Index 2

    myGraph.addEdge(v0, v1);
    myGraph.setEdgeValue(v0, v1, 300.5);
    myGraph.addEdge(v0, v2);
    myGraph.setEdgeValue(v0, v2, 450.0);

    std::cout << "Sasiaduje 0 z 1? (Krakow-Warszawa): "
              << (myGraph.adjacent(v0, v1) ? "TAK" : "NIE") << "\n";

    std::cout << "\nUsuwamy Warszawe (index 1). Poznan (index 2) powinien wskoczyc na jej miejsce!\n";
    myGraph.removeVertex(v1);

    std::cout << "Sasiaduje 0 z 1? (Teraz to Krakow-Poznan): "
              << (myGraph.adjacent(v0, 1) ? "TAK" : "NIE") << "\n";

    if (myGraph.adjacent(v0, 1)) {
        std::cout << "Wartosc nowej krawedzi (powinno byc 450): "
                  << myGraph.getEdgeValue(v0, 1) << "\n";
    }

    std::cout << "\n--- BENCHMARK ---\n";
    benchmark();

    return 0;
}