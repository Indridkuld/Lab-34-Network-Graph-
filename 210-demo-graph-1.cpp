#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <queue>

using namespace std;

const int SIZE = 9;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

const vector<string> STATION_NAMES = {
    "Central Hub",        // 0
    "Museum District",    // 1
    "Tech Park",          // 2
    "Old Town",           // 3
    "University",         // 4
    "Airport",            // 5
    "Business Park",      // 6
    "Suburban Terminal",  // 7
    "Stadium"             // 8
};

class Graph {
public:
    vector<vector<Pair>> adjList;

    Graph(vector<Edge> const &edges) {
        adjList.resize(SIZE);

        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            adjList[src].push_back(make_pair(dest, weight));
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < (int)adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }

    void printTransitNetwork() {
    cout << "City Metro Network Topology:\n";
    cout << "============================\n";
    for (int i = 0; i < (int)adjList.size(); ++i) {
        cout << "Station " << i << " (" << STATION_NAMES[i] << ") connects to:\n";
        for (auto &p : adjList[i]) {
            int v = p.first;
            int w = p.second;
            cout << "  → Station " << v << " (" << STATION_NAMES[v]
                 << ") - Travel time: " << w << " min\n";
        }
        cout << endl;
    }
}

    // Depth-First Search (iterative)
    void dfs(int start) {
        vector<bool> visited(SIZE, false);
        stack<int> st;
        st.push(start);

        cout << "DFS starting from vertex " << start << ":" << endl;

        while (!st.empty()) {
            int u = st.top();
            st.pop();

            if (visited[u]) continue;
            visited[u] = true;

            cout << u << " ";

            // Push neighbors in the stored order
            for (Pair p : adjList[u]) {
                int v = p.first;
                if (!visited[v]) {
                    st.push(v);
                }
            }
        }
        cout << endl;
    }

    // Breadth-First Search
    void bfs(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        cout << "BFS starting from vertex " << start << ":" << endl;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            cout << u << " ";

            for (Pair p : adjList[u]) {
                int v = p.first;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << endl;
    }

    vector<int> shortestPathsFrom(int start) {
        const int INF = numeric_limits<int>::max();
        vector<int> dist(SIZE, INF);
        dist[start] = 0;

        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;

            for (auto &p : adjList[u]) {
                int v = p.first;
                int w = p.second;
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        return dist;
    }
};

int main() {
    // (x, y, w) —> edge from x to y having weight w
    vector<Edge> edges = {
    {0,1,8},  {0,2,21},
    {1,2,6},  {1,3,5},  {1,4,4},
    {2,7,11}, {2,8,8},
    {3,4,9},
    {5,6,10}, {5,7,15}, {5,8,5},
    {6,7,3},  {6,8,7}
    };

    Graph graph(edges);

    graph.printGraph();
    cout << "Metro Route Exploration (DFS) from Station 0 (Central Hub):\n";
    graph.dfs(0);
    cout << "\nLayer-by-Layer Reach (BFS) from Station 0 (Central Hub):\n";
    graph.bfs(0);

    auto dist = graph.shortestPathsFrom(0);
    cout << "Shortest path from node 0:\n";
    for (int i = 0; i < SIZE; ++i) {
        cout << "0 -> " << i << " : " << dist[i] << "\n";
    }

    return 0;
}
