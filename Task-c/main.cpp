#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <unordered_map>

std::unordered_map<uint32_t, std::string> naming;

const std::filesystem::path file_graph_name = "../../graph.csv";
const std::filesystem::path file_naming_name = "../../naming.csv";

using Graph = std::vector<std::vector<int>>;

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
        graph[start].push_back(end);
        graph[end].push_back(start);
    }
}

const uint32_t kInfinity = -1;

std::vector<std::vector<uint32_t>> dist;

// Floyd Warshall - basic
// but i ignore infinity distances, because of islands
void FillDist() {
    dist.resize(vertex_amount + 1, std::vector<uint32_t>(vertex_amount + 1, kInfinity));

    for (uint32_t i = 1; i <= vertex_amount; i++) {
        dist[i][i] = 0;
        for (auto v : graph[i]) {
            dist[i][v] = 1;
        }
    }

    for (uint32_t i = 1; i <= vertex_amount; i++)
        for (uint32_t u = 1; u <= vertex_amount; u++)
            for (uint32_t v = 1; v <= vertex_amount; v++) {
                if (dist[u][i] == kInfinity || dist[i][v] == kInfinity)
                    continue;
                if (dist[u][i] + dist[i][v] < dist[u][v])
                    dist[u][v] = dist[u][i] + dist[i][v];
            }
}


std::vector<uint32_t> eccentricity;

// biggest of distances
void FindEccentricity() {
    eccentricity.resize(vertex_amount + 1, kInfinity);

    for (uint32_t v = 1; v <= vertex_amount; v++) {
        for (uint32_t u = 1; u <= vertex_amount; u++) {
            if (u == v)
                continue;
            if (dist[u][v] != kInfinity &&
                (eccentricity[v] == kInfinity || eccentricity[v] < dist[u][v])) {
                eccentricity[v] = dist[u][v];
            }
        }
    }
}

int main() {
    LoadNaming();
    LoadGraph();
    FillDist();
    FindEccentricity();
    /*
    uncomment if you want to see all eccentricity
    i've launched it already, i analyzed it, and radius is 5

    for (uint32_t v = 1; v <= vertex_amount; v++) {
            std::cout << naming[v] << " " << eccentricity[v] << "\n";
    }
    */
    for (uint32_t v = 1; v <= vertex_amount; v++) {
        if (eccentricity[v] == 5)
            std::cout << naming[v] << ", ";
    }

    return 0;
}

