#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <set>
#include <map>

std::unordered_map<uint32_t, std::string> naming;
std::unordered_map<std::string, uint32_t> naming_reverse;

const std::filesystem::path file_graph_name = "../../graph.csv";
const std::filesystem::path file_naming_name = "../../naming.csv";
const std::filesystem::path file_distance_name = "../../edges.txt";

using Graph = std::vector<std::map<uint32_t, uint32_t>>;

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
        naming_reverse.insert({name, v});
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
        graph[start].insert({end, 0});
        graph[end].insert({start, 0});
    }
}

void LoadDistances() {
    std::ifstream file_in(file_distance_name);
    if (!file_in.is_open())
        throw std::out_of_range("");

    std::string start_s, end_s;
    uint32_t start, end;
    uint32_t dist;

    while (file_in >> start_s) {
        file_in >> end_s >> end_s; // to skip '--'
        file_in >> dist;

        start = naming_reverse[start_s];
        end = naming_reverse[end_s];

        graph[start].find(end)->second = dist;
        graph[end].find(start)->second = dist;
    }
}

bool DFSCycleCheck(const Graph& g, std::vector<bool>& was, uint32_t v, uint32_t p) {
    if (was[v])
        return true;
    was[v] = true;

    for (auto u : g[v]) {
        if (u.first == p)
            continue;
        if (DFSCycleCheck(g, was, u.first, v)) {
            return true;
        }
    }

    return false;
}

bool CheckForCycle(const Graph& g) {
    std::vector<bool> was(g.size(), false);
    uint32_t i = 0;
    while (true) {
        if (DFSCycleCheck(g, was, i, -1))
            return true;
        while (i <= vertex_amount && was[i])
            i++;
        if (i >= vertex_amount)
            break;
    }
    return false;
}

Graph SpanningTree() {
    std::set<std::pair<uint32_t, std::pair<uint32_t, uint32_t>>> edges;

    Graph ans(vertex_amount + 1);

    for (int v = 1; v <= vertex_amount; v++) {
        for (auto& u : graph[v]) {
            edges.insert({u.second, {u.first, v}});
        }
    }

    uint32_t added = 0;
    uint32_t weight = 0;

    while (!edges.empty()) {
        while (!edges.empty()) {
            auto edge = *edges.begin();

            Graph mb = ans;
            if (mb[edge.second.first].find(edge.second.second) != mb[edge.second.first].end()){
                edges.erase(edges.begin());
                continue;
            }
            mb[edge.second.first].insert({edge.second.second, 0});
            mb[edge.second.second].insert({edge.second.first, 0});

            if (!CheckForCycle(mb)) {
                weight += edges.begin()->first;
                ans = mb;
                edges.erase(edges.begin());
                break;
            }
            edges.erase(edges.begin());
        }
        added++;
    }

    std::cout << "Create tree with weight: " << weight << "\n";

    return ans;
}

std::ostream& operator<<(std::ostream& os, Graph g) {
    for (int i = 1; i <= vertex_amount; ++i) {
        for (auto& j : g[i]) {
            os << naming[i] << " -- " << naming[j.first] << "\n";
        }
    }
    return os;
}

int main() {
    LoadNaming();
    LoadGraph();
    LoadDistances();
    std::cout << SpanningTree();
    std::cout << SpanningTree().size();
    return 0;
}

