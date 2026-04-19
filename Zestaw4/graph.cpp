#include <iostream>
#include <vector>
#include <random>
#include <chrono>

template <typename V, typename E>
class Graph {

    struct Edge {
        bool is_connected = false;
        E value{};
    };

    std::vector<std::vector<Edge>> adjMatrix;
    std::vector<V> vertices;

public:
    Graph() = default;

    bool adjacent(int x, int y) const {
        if (x >= vertices.size() || y >= vertices.size()) throw std::out_of_range("Vertex out of range");
        return adjMatrix[x][y].is_connected;
    }

    std::vector<int> neighbours(int x) const {
        std::vector<int> neighbours;

        for (int i = 0; i < vertices.size(); ++i) {
            if (adjMatrix[x][i].is_connected) {
                neighbours.push_back(i);
            }
        }
        return neighbours;
    }

    int addVertex(V val) {
        vertices.push_back(val);
        int newSize = vertices.size();

        for (auto& row : adjMatrix) {
            row.resize(newSize);
        }

        adjMatrix.emplace_back(newSize);

        return newSize - 1;
    }

    void removeVertex(int x) {
        if (x >= vertices.size()) return;

        vertices.erase(vertices.begin() + x);
        adjMatrix.erase(adjMatrix.begin() + x);

        for (auto& row : adjMatrix) {
            row.erase(row.begin() + x);
        }
    }

    void addEdge(int x, int y) {
        if (x >= vertices.size() || y >= vertices.size()) return;
        adjMatrix[x][y].is_connected = true;
    }

    void removeEdge(int x, int y) {
        if (x >= vertices.size() || y >= vertices.size()) return;
        adjMatrix[x][y].is_connected = false;
    }

    V getVertexValue(int x) const {
        return vertices[x];
    }

    void setVertexValue(int x, V v) {
        vertices.at(x) = v;
    }

    E getEdgeValue(int x, int y) const {
        if (!adjMatrix.at(x).at(y).is_connected) throw std::out_of_range("Edge not connected");
        return adjMatrix[x][y].value;
    }

    void setEdgeValue(int x, int y, E v) {
        if (!adjMatrix.at(x).at(y).is_connected) throw std::out_of_range("Edge not connected");
        adjMatrix[x][y].value = v;
    }

};

// --- BADANIE ZŁOŻONOŚCI DLA adjacent(x, y) ---
void benchmark() {
    std::cout << "Rozmiar(V),Czas_adjacent(ns)\n";
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int v = 1000; v <= 10000; v += 1000) {
        Graph<int, int> G;
        for (int i = 0; i < v; ++i) G.addVertex(i);

        std::uniform_int_distribution<> dist(0, v - 1);
        int x = dist(gen);
        int y = dist(gen);

        auto start = std::chrono::high_resolution_clock::now();
        G.adjacent(x, y);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << v << "," << duration << "\n";
    }
}

int main() {
    std::cout << "--- Test Grafu ---\n";
    Graph<std::string, double> myGraph;
    int v1 = myGraph.addVertex("Krakow");
    int v2 = myGraph.addVertex("Warszawa");

    myGraph.addEdge(v1, v2);
    myGraph.setEdgeValue(v1, v2, 299.5);

    std::cout << "Czy sasiaduja? " << (myGraph.adjacent(v1, v2) ? "Tak" : "Nie") << "\n";
    std::cout << "Dystans: " << myGraph.getEdgeValue(v1, v2) << "\n\n";

    std::cout << "--- Benchmark ---\n";
    benchmark();

    return 0;
}