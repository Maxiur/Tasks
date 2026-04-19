#include <iostream>

class Graph {

};

void exportToDot(const std::string& filename, Graph graph) const {
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