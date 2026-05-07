#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

const int MAX_NODES = 1000;


void generateGraph(vector<vector<int>>& adjList, int numNodes) {
    for (int i = 0; i < numNodes; i ++) {
        for (int j = 0; j < numNodes; j++){
            if(rand()%2){
                adjList[i].push_back(j);
                adjList[j].push_back(i);
            }
        }
    }
}

// Function to print the graph as an adjacency list
void printGraph(const vector<vector<int>>& adjList) {
    cout << "Graph (Adjacency List Representation):\n";
    for (size_t i = 0; i < adjList.size(); ++i) {
        cout << "Node " << i << ": ";
        for (int neighbor : adjList[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void bfsSequential(const vector<vector<int>>& adjList, int startNode) {
    vector<bool> visited(adjList.size(), false);
    queue<int> q;
    q.push(startNode);
    visited[startNode] = true;

    cout << "BFS Sequential Order: ";
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        // cout << node << " ";  // Print visited node
        int k = 0;
        for(int i : adjList[node]){
            k += i;
        }
        for (int neighbor : adjList[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    cout << endl;
}

void dfsSequential(const vector<vector<int>>& adjList, int startNode) {
    vector<bool> visited(adjList.size(), false);
    stack<int> s;
    s.push(startNode);
    visited[startNode] = true;

    cout << "DFS Sequential Order: ";
    while (!s.empty()) {
        int node = s.top();
        s.pop();
        // cout << node << " ";  // Print visited node
        int k = 0;
        for(int i : adjList[node]){
            k += i;
        }
        for (int neighbor : adjList[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                s.push(neighbor);
            }
        }
    }
    cout << endl;
}

void bfsParallel(const vector<vector<int>>& adjList, int startNode) {
    vector<bool> visited(adjList.size(), false);
    queue<int> q;
    q.push(startNode);
    visited[startNode] = true;

    cout << "BFS Parallel Order: ";
    // Parallelize the marking of visited nodes
    #pragma omp parallel
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        // cout << node << " ";  // Print visited node

            #pragma omp parallel for
            for (size_t i = 0; i < adjList[node].size(); ++i) {
                int neighbor = adjList[node][i];
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
    }
    cout << endl;
}

void dfsParallel(const vector<vector<int>>& adjList, int startNode) {
    vector<bool> visited(adjList.size(), false);
    stack<int> s;
    s.push(startNode);
    visited[startNode] = true;

    cout << "DFS Parallel Order: ";
    // Parallelize the marking of visited nodes
    #pragma omp parallel
    while (!s.empty()) {
        int node = s.top();
        s.pop();
        // cout << node << " ";  // Print visited node
            #pragma omp parallel for
            for (size_t i = 0; i < adjList[node].size(); ++i) {
                int neighbor = adjList[node][i];
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        s.push(neighbor);
                    }
                }
            }
    }
    cout << endl;
}

int main() {
    int nodes = 100;
    cout << "enter : ";
cin >> nodes;
    vector<vector<int>> adjList(nodes);
    generateGraph(adjList, nodes);

    // Print the generated graph
    // printGraph(adjList);

    auto start = high_resolution_clock::now();
    bfsSequential(adjList, 0);
    auto end = high_resolution_clock::now();
    double duration_seq = duration_cast<nanoseconds>(end - start).count();
    cout << "BFS Sequential Time Taken: " << duration_seq << " ns" << endl << endl;

    start = high_resolution_clock::now();
    bfsParallel(adjList, 0);
    end = high_resolution_clock::now();
    double duration_par = duration_cast<nanoseconds>(end - start).count();
    cout << "BFS Parallel Time Taken: " << duration_par << " ns" << endl << endl;

    cout << "Speedup Factor for BFS: " << duration_seq / duration_par << "\n\n";

    start = high_resolution_clock::now();
    dfsSequential(adjList, 0);
    end = high_resolution_clock::now();
    duration_seq = duration_cast<nanoseconds>(end - start).count();
    cout << "DFS Sequential Time Taken: " << duration_seq << " ns" << endl << endl;

    start = high_resolution_clock::now();
    dfsParallel(adjList, 0);
    end = high_resolution_clock::now();
    duration_par = duration_cast<nanoseconds>(end - start).count();
    cout << "DFS Parallel Time Taken: " << duration_par << " ns" << endl << endl;

    cout << "Speedup Factor for DFS: " << duration_seq / duration_par << "\n\n";

    return 0;
}
