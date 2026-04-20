#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

template <typename V, typename E>
class Graph {
private:
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

        int lastIndex = vertices.size() - 1;

        if (x != lastIndex) {
            // Zamieniamy same wierzchołki
            std::swap(vertices[x], vertices[lastIndex]);

            // Zamieniamy całe wiersze w macierzy
            std::swap(adjMatrix[x], adjMatrix[lastIndex]);

            // Zamieniamy kolumny w każdym wierszu
            for (auto& row : adjMatrix) {
                std::swap(row[x], row[lastIndex]);
            }
        }

        vertices.pop_back();
        adjMatrix.pop_back();

        for (auto& row : adjMatrix) {
            row.pop_back(); // Usuwa ostatnią kolumnę z pozostałych wierszy
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


    void exportToDot(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Nie da sie otworzyc pliku do zapisu!");
        }

        file << "digraph G {\n";

        file << "  // Wierzcholki\n";
        for (int i = 0; i < vertices.size(); ++i) {
            file << "  " << i << " [label=\"" << vertices[i] << "\"];\n";
        }

        file << "\n  // Krawedzie\n";
        for (int i = 0; i < vertices.size(); ++i) {
            for (int j = 0; j < vertices.size(); ++j) {
                if (adjMatrix[i][j].is_connected) {
                    file << "  " << i << " -> " << j
                         << " [label=\"" << adjMatrix[i][j].value << "\"];\n";
                }
            }
        }

        file << "}\n";
        file.close();
    }
};

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

    myGraph.exportToDot("../grafMiasta.dot");

    // std::cout << "--- Benchmark ---\n";
    // benchmark();

    Graph<std::string, int> triGraph;
    int a = triGraph.addVertex("A");
    int b = triGraph.addVertex("B");
    int c = triGraph.addVertex("C");

    // Łączymy A <-> B, B <-> C, C <-> A
    triGraph.addEdge(a, b); triGraph.addEdge(b, a);
    triGraph.addEdge(b, c); triGraph.addEdge(c, b);
    triGraph.addEdge(c, a); triGraph.addEdge(a, c);

    triGraph.setEdgeValue(a, b, 1); triGraph.setEdgeValue(b, a, 1);
    triGraph.setEdgeValue(b, c, 2); triGraph.setEdgeValue(c, b, 2);
    triGraph.setEdgeValue(c, a, 3); triGraph.setEdgeValue(a, c, 3);

    std::cout << "Generowanie pliku graf.dot...\n";
    triGraph.exportToDot("../graf.dot");

    std::cout << "Gotowe.\n";

    return 0;
}