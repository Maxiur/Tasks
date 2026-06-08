#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <filesystem>
#include <iomanip>

const int INF = 1e7;

struct City {
    std::string name;
    double lat;
    double lon;
};

int main() {
    // 1. Ładowanie współrzędnych
    std::ifstream pos_file("../positions");
    if (!pos_file.is_open()) {
        std::cerr << "Brak pliku positions!\n";
        return 1;
    }

    std::map<std::string, int> city_to_idx;
    std::vector<City> cities;
    std::string name;
    double lat, lon;

    while (pos_file >> name >> lat >> lon) {
        city_to_idx[name] = cities.size();
        cities.push_back({name, lat, lon});
    }
    pos_file.close();

    int n = cities.size();
    if (n == 0) {
        std::cerr << "Pusto w positions.txt, sprawdz to.\n";
        return 1;
    }

    // 2. Ładowanie czasów przejazdu
    std::ifstream time_file("../czasy");
    if (!time_file.is_open()) {
        std::cerr << "Brak pliku czasy.txt!\n";
        return 1;
    }

    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
    std::vector<std::vector<int>> nxt(n, std::vector<int>(n, -1));

    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
        nxt[i][i] = i;
    }

    std::string u_name, v_name;
    int w;
    while (time_file >> u_name >> v_name >> w) {
        if (city_to_idx.contains(u_name) && city_to_idx.contains(v_name)) {
            int u = city_to_idx[u_name];
            int v = city_to_idx[v_name];
            dist[u][v] = w;
            dist[v][u] = w;
            nxt[u][v] = v;
            nxt[v][u] = u;
        }
    }
    time_file.close();

    // 3. Algorytm Floyda-Warshalla z odtwarzaniem trasy
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    nxt[i][j] = nxt[i][k];
                }
            }
        }
    }

    // 4. Generowanie plików DOT
    std::string out_dir = "../wykresy_dot";
    std::filesystem::create_directory(out_dir);

    // Skrypt bash/bat do zautomatyzowania renderowania (dla wygody)
    std::ofstream script(out_dir + "../render_all.sh");
    script << "#!/bin/bash\n";

    int files_created = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) { // i < j żeby nie robić duplikatów
            if (nxt[i][j] == -1) continue; // Brak trasy (nie powinno się zdarzyć w spójnym grafie)

            std::string filename = cities[i].name + "_do_" + cities[j].name + ".dot";
            std::string filepath = out_dir + "/" + filename;
            std::ofstream dot(filepath);

            dot << "graph G {\n";
            dot << "    node [shape=point, color=blue, width=0.1];\n";
            dot << "    edge [color=red, penwidth=2];\n";

            // Definiujemy wszystkie wierzchołki (z ich współrzędnymi geograficznymi)
            for (const auto& city : cities) {
                // Program dot (fdp) uzywa układu X, Y. Lon to X, Lat to Y.
                dot << "    \"" << city.name << "\" [pos=\""
                    << city.lon << "," << city.lat << "!\", xlabel=\"" << city.name << "\"];\n";
            }

            // Odtwarzamy trasę między miastem 'i' a miastem 'j'
            int curr = i;
            while (curr != j) {
                int next_node = nxt[curr][j];
                dot << "    \"" << cities[curr].name << "\" -- \"" << cities[next_node].name << "\";\n";
                curr = next_node;
            }

            dot << "}\n";
            dot.close();

            // Dodajemy komendę renderowania do skryptu
            std::string png_name = cities[i].name + "_do_" + cities[j].name + ".png";
            script << "dot -Kfdp -n -Tpng " << filename << " -o " << png_name << "\n";
            files_created++;
        }
    }
    script.close();

    return 0;
}
