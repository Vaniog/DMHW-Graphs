#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <set>

std::unordered_map<uint32_t, std::string> naming;

const std::filesystem::path file_graph_name = "../../graph.csv";
const std::filesystem::path file_naming_name = "../../naming.csv";

using Graph = std::vector<std::set<int>>;

uint32_t vertex_amount;
uint32_t edges_amount;
Graph graph;

// for converting vertex id to name
void LoadNaming() {
    std::ifstream file_in(file_naming_name);

    uint32_t v;
    std::string name;
    while (file_in >> v) {
        file_in >> name;
        naming.insert({v, name});
    }
};

// soo basic, soo obviously
void LoadGraph() {
    std::ifstream file_in(file_graph_name);
    if (!file_in.is_open())
        throw std::out_of_range("");

    file_in >> vertex_amount >> edges_amount;

    graph.resize(vertex_amount + 1);

    for (uint32_t i = 0; i < edges_amount; i++) {
        int start, end;
        file_in >> start >> end;
        graph[start].insert(end);
        graph[end].insert(start);
    }
}

bool IsClique(const std::vector<int>& vs) {
    for (int i = 0; i < vs.size(); i++) {
        for (int j = 0; j < vs.size(); j++) {
            if (j != i && graph[vs[i]].find(vs[j]) == graph[vs[i]].end())
                return false;
        }
    }

    return true;
}

int main() {
    LoadNaming();
    LoadGraph();

    for (int v1 = 0; v1 < graph.size(); v1++) {
        for (int v2 = v1 + 1; v2 < graph.size(); v2++) {
            for (int v3 = v2 + 1; v3 < graph.size(); v3++) {
                for (int v4 = v3 + 1; v4 < graph.size(); v4++) {

                    std::vector<int> vs4 = {v1, v2, v3, v4};

                    if (IsClique(vs4)) {
                        std::cout << "[";
                        for (auto v : vs4) {
                            std::cout << naming[v] << ", ";
                        }
                        std::cout << "]\n";
                    }

                    for (int v5 = v4 + 1; v5 < graph.size(); v5++) {

                        std::vector<int> vs5 = {v1, v2, v3, v4, v5};

                        if (IsClique(vs5)) {
                            for (auto v : vs5) {
                                std::cout << naming[v] << " ";
                            }
                            std::cout << "\n";
                        }

                    }
                }
            }
        }
    }

    return 0;
}

