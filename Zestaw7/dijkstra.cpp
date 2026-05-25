#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <tuple>
#include <algorithm>

// Zwracamy parę wektorów: [dystanse, rodzice]
std::pair<std::vector<int>, std::vector<int>> dijkstra(const std::vector<std::vector<std::pair<int, int>>>& adj, int src) {
    int V = adj.size();
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    std::vector<int> dist(V, INT_MAX);
    std::vector<int> parent(V, -1);

    dist[src] = 0;
    pq.emplace(0, src);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }
    return {dist, parent};
}

int main() {
    int numNodes = 10;
    std::vector<std::vector<std::pair<int, int>>> adj(numNodes);

    std::vector<std::tuple<int, int, int>> edges = {
        {0, 1, 3}, {1, 2, 2}, {1, 3, 1}, {3, 4, 1},
        {4, 5, 1}, {5, 6, 1}, {5, 7, 2}, {6, 7, 1},
        {2, 7, 2}, {7, 8, 2}, {7, 9, 1}
    };

    for (const auto& [u, v, w] : edges) {
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    // --- CZĘŚĆ A: Z klatki schodowej (A = 0) ---
    auto [distA, parentA] = dijkstra(adj, 0);
    std::cout << "--- CZESC A ---\n";
    std::cout << "Najkrotszy czas A -> J: " << distA[9] << "\n";
    std::cout << "Najkrotszy czas A -> I: " << distA[8] << "\n";
    std::cout << "Najkrotszy czas A -> H: " << distA[7] << "\n";
    std::cout << "Najkrotszy czas A -> G: " << distA[6] << "\n";
    std::cout << "Najkrotszy czas A -> F: " << distA[5] << "\n";
    std::cout << "Najkrotszy czas A -> E: " << distA[4] << "\n";
    std::cout << "Najkrotszy czas A -> D: " << distA[3] << "\n";
    std::cout << "Najkrotszy czas A -> C: " << distA[2] << "\n";
    std::cout << "Najkrotszy czas A -> B: " << distA[1] << "\n";

    auto [distJ, parentJ] = dijkstra(adj, 9);

    std::cout << "\n--- POWROT Z UBIKACJI (J) ---\n";
    std::cout << "Czas do ubikacji J -> I : " << distJ[8] << "\n";
    std::cout << "Czas do ubikacji J -> H : " << distJ[7] << "\n";
    std::cout << "Czas do ubikacji J -> G : " << distJ[6] << "\n";
    std::cout << "Czas do ubikacji J -> F : " << distJ[5] << "\n";
    std::cout << "Czas do ubikacji J -> E : " << distJ[4] << "\n";
    std::cout << "Czas do ubikacji J -> D : " << distJ[3] << "\n";
    std::cout << "Czas do ubikacji J -> C : " << distJ[2] << "\n";
    std::cout << "Czas do ubikacji J -> B : " << distJ[1] << "\n";
    std::cout << "Czas do ubikacji J -> A : " << distJ[0] << "\n";

    // --- CZĘŚĆ B: Z aneksu (I = 8) do szefa (D = 3) ---
    auto [distI, parentI] = dijkstra(adj, 8);
    std::cout << "\n--- CZESC B ---\n";
    std::cout << "Najkrotszy czas z aneksu (I) do szefa (D): " << distI[3] << "\n";

    // Odtwarzanie ścieżki
    std::vector<int> path;
    int curr = 3; // Startujemy od tyłu (od szefa)
    while (curr != -1) {
        path.push_back(curr);
        curr = parentI[curr];
    }
    std::ranges::reverse(path);

    std::cout << "Trasa: ";
    for (std::size_t i = 0; i < path.size(); ++i) {
        std::cout << static_cast<char>(path[i] + 'A') << (i < path.size() - 1 ? " -> " : "");
    }
    std::cout << "\n";

    return 0;
}