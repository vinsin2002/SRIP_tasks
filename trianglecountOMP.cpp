#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <omp.h>

using namespace std;

class Graph {
public:
    int V;
    vector<unordered_set<int>> adjList;

    Graph(int V) {
        this->V = V;
        adjList.resize(V);
    }

    void addEdge(int u, int v) {
        adjList[u].insert(v);
        adjList[v].insert(u);
    }

    int countTriangles() {
        int triangleCount = 0;
        #pragma omp parallel for reduction(+:triangleCount)
        for (int i = 0; i < V; i++) {
            for (int j : adjList[i]) {
                if (j > i) {
                    for (int k : adjList[j]) {
                        if (k > j && adjList[i].find(k) != adjList[i].end()) {
                            triangleCount++;
                        }
                    }
                }
            }
        }
        return triangleCount;
    }
};

int determineMaxNodeID(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        exit(1);
    }

    string line;
    int maxNodeID = 0;
    while (getline(file, line)) {
        if (line[0] == '#') continue;
        stringstream ss(line);
        int u, v;
        ss >> u >> v;
        maxNodeID = max(maxNodeID, max(u, v));
    }

    file.close();
    return maxNodeID;
}

void loadGraph(const string &filename, Graph &graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line[0] == '#') continue;
        stringstream ss(line);
        int u, v;
        ss >> u >> v;
        graph.addEdge(u, v);
    }

    file.close();
}

int main() {
    string filename = "roadNet-PA.txt";
    cout << "Determining maximum node ID..." << endl;
    int maxNodeID = determineMaxNodeID(filename);
    int V = maxNodeID + 1;

    cout << "Number of nodes in the graph: " << V << endl;

    Graph g(V);

    cout << "Loading graph..." << endl;
    loadGraph(filename, g);
    cout << "Graph loaded. Starting triangle count..." << endl;
    double starttime = omp_get_wtime();
    int triangleCount = g.countTriangles();
    double endtime = omp_get_wtime();
    cout << "Number of triangles in the graph: " << triangleCount << endl;
    cout << "Time required to calculate number of triangles: " << endtime-starttime << "s" << endl;
    return 0;
}