#include <iostream>
#include <vector>
#include <queue>
#include <utility>

int main() {
    std::vector<std::pair<int, int> > graph = {{0, 1}, {1, 3}, {2, 3}, {3, 5}, {3, 7}, {4, 5}, {5, 6}, {6, 8}, {7, 8}};
    int numNodes = 9;
    std::vector<std::vector<int> > adjList(numNodes);

    std::vector<int> answer;

    std::vector<int> indegree(numNodes, 0);
    for (const auto& [u, v] : graph) {
        adjList[u].push_back(v);
        indegree[v]++;
    }

    std::queue <int> queue;
    for (int i = 0; i < indegree.size(); i++) {
        if (indegree[i] == 0) queue.push(i);
    }

    while (!queue.empty()) {
        int node = queue.front();
        queue.pop();
        answer.push_back(node);

        for (auto nei : adjList[node]) {
            indegree[nei]--;
            if (indegree[nei] == 0) queue.push(nei);
        }
    }

    for (auto i : answer) {
        std::cout << i << " ";
    }

    return 0;
}