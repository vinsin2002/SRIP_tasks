#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <fstream>
#include <sstream>
#include <omp.h>

typedef long long ll;

using namespace std;

class Graph_unordered_set {
public:
    int V;
    vector<unordered_set<int>> adjList;

    Graph_unordered_set(int V) {
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
class Graph_set {
public:
    int V;
    vector<set<int>> adjList;

    Graph_set(int V) {
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
void countDegree(const string &filename,vector<int> &degree) {
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
        degree[u]++; degree[v]++;
    }

    file.close();
}

void loadGraph(const string &filename, Graph_set &graph) {
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
void loadGraph(const string &filename, Graph_unordered_set &graph) {
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
pair<int,int> getMaxAvg(vector<int> &degree,int V){
        int maxval = 0;
        int avgval = 0;
        ll sum = 0;
        for(int i=0;i<V;i++)
        {
                maxval = max(maxval,degree[i]);
                sum += degree[i];
        }
        avgval = sum/V;
        pair<int,int> result = {maxval,avgval};
        return result;
}

int main() {
    string filename = "./../graphfiles/roadNet-TX.txt";
    cout << "Determining maximum node ID..." << endl;
    int maxNodeID = determineMaxNodeID(filename);
    int V = maxNodeID + 1;

    cout << "Number of nodes in the graph: " << V << endl;
    vector<int> degree(V,0);
    countDegree(filename,degree);
    pair<int,int> maxandavg = getMaxAvg(degree,V);
    int maxdegree = maxandavg.first;
    int avgdegree = maxandavg.second;
    int diff = maxdegree-avgdegree;
    cout<<"Max Degree in the graph for a Vertex : "<<maxdegree<<endl;
    cout<<"Avg Degree in the graph for Vertices : "<<avgdegree<<endl;
    cout<<"Difference between Max and Avg degrees : "<<diff<<endl;
    if(diff > 100){
        cout<<"Using Hash Tables"<<endl;
        Graph_unordered_set g(V);
        cout << "Loading graph..." << endl;
        loadGraph(filename, g);
        cout << "Graph loaded. Starting triangle count..." << endl;
        double starttime = omp_get_wtime();
        int triangleCount = g.countTriangles();
        double endtime = omp_get_wtime();
        cout << "Number of triangles in the graph: " << triangleCount << endl;
        cout << "Time required to calculate number of triangles: " << endtime-starttime << "s" << endl;
   }
    else{
        cout<<"Using Binary Search Trees"<<endl;
        Graph_set g(V);
        cout << "Loading graph..." << endl;
        loadGraph(filename, g);
        cout << "Graph loaded. Starting triangle count..." << endl;
        double starttime = omp_get_wtime();
        int triangleCount = g.countTriangles();
        double endtime = omp_get_wtime();
        cout << "Number of triangles in the graph: " << triangleCount << endl;
        cout << "Time required to calculate number of triangles: " << endtime-starttime << "s" << endl;
     }
    return 0;
}