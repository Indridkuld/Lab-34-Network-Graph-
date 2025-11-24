#include <iostream>
#include <vector>
#include <stack>
#include <limits>
#include <queue>
#include <tuple>
#include <functional>
#include <string>
#include <algorithm>

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
            adjList[dest].push_back(make_pair(src, weight)); // undirected
        }

        // Sort adjacency lists so printed order is consistent
        for (auto &neighbors : adjList) {
            sort(neighbors.begin(), neighbors.end());
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

        cout << "DFS starting from station " << start
             << " (" << STATION_NAMES[start] << "):" << endl;

        while (!st.empty()) {
            int u = st.top();
            st.pop();

            if (visited[u]) continue;
            visited[u] = true;

            cout << u << " (" << STATION_NAMES[u] << ")  ";

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

        cout << "BFS starting from station " << start
             << " (" << STATION_NAMES[start] << "):" << endl;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            cout << u << " (" << STATION_NAMES[u] << ")  ";

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

    // Dijkstra shortest paths
    vector<int> shortestPathsFrom(int start) {
        const int INF = numeric_limits<int>::max();
        vector<int> dist(SIZE, INF);
        dist[start] = 0;

        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            Pair top = pq.top();
            pq.pop();
            int d = top.first;
            int u = top.second;

            if (d > dist[u]) continue;

            for (auto &p : adjList[u]) {
                int v = p.first;
                int w = p.second;
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        return dist;
    }

    // Prim's MST
    vector<tuple<int,int,int>> minimumSpanningTree(int start = 0) {
        vector<tuple<int,int,int>> mstEdges; // (u, v, w)
        vector<bool> inMST(SIZE, false);

        typedef tuple<int,int,int> EdgeTuple; // (weight, u, v)

        priority_queue<
            EdgeTuple,
            vector<EdgeTuple>,
            greater<EdgeTuple>
        > pq;

        inMST[start] = true;
        for (auto &p : adjList[start]) {
            pq.push(make_tuple(p.second, start, p.first));
        }

        while (!pq.empty() && (int)mstEdges.size() < SIZE - 1) {
            EdgeTuple t = pq.top();
            pq.pop();
            int w = get<0>(t);
            int u = get<1>(t);
            int v = get<2>(t);

            if (inMST[v]) continue;

            inMST[v] = true;
            mstEdges.push_back(make_tuple(u, v, w));

            for (auto &p : adjList[v]) {
                int to = p.first;
                int wt = p.second;
                if (!inMST[to]) {
                    pq.push(make_tuple(wt, v, to));
                }
            }
        }

        return mstEdges;
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

    int choice;
    do {
        cout << "\nCity Metro Network Menu:\n";
        cout << "[1] Display metro network\n";
        cout << "[2] Check coverage (BFS from Central Hub)\n";
        cout << "[3] Plan inspection route (DFS from Central Hub)\n";
        cout << "[4] Calculate shortest travel times from Central Hub\n";
        cout << "[5] Find Minimum Spanning Tree of the network\n";
        cout << "[0] Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nGraph adjacency list:\n";
                graph.printGraph();
                cout << "\nReal-world metro view:\n";
                graph.printTransitNetwork();
                break;

            case 2:
                cout << "\nLayer-by-layer coverage (BFS):\n";
                graph.bfs(0);
                break;

            case 3:
                cout << "\nDepth-first route (DFS):\n";
                graph.dfs(0);
                break;

            case 4: {
                auto dist = graph.shortestPathsFrom(0);
                cout << "\nShortest travel times from Station 0 (Central Hub):\n";
                for (int i = 0; i < SIZE; ++i) {
                    cout << "0 -> " << i << " (" << STATION_NAMES[i]
                         << ") : " << dist[i] << " minutes\n";
                }
                break;
            }

            case 5: {
                auto mst = graph.minimumSpanningTree(0);
                cout << "\nMinimum Spanning Tree edges (Metro backbone):\n";
                for (auto &e : mst) {
                    int u = get<0>(e);
                    int v = get<1>(e);
                    int w = get<2>(e);
                    cout << "Edge from Station " << u << " ("
                         << STATION_NAMES[u] << ") to Station " << v
                         << " (" << STATION_NAMES[v]
                         << ") with travel time: " << w << " minutes\n";
                }
                break;
            }

            case 0:
                cout << "Exiting Metro Network application.\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
