#include <bits/stdc++.h> 
using namespace std; 

vector<vector<pair<int, int>>> constAdj(vector<vector<int>> &edges, int n) { 
    vector<vector<pair<int, int>>> adj(n); 
    for (auto edge : edges) { 
        int u = edge[0], v = edge[1], wt = edge[2]; 
        adj[u].push_back({v, wt}); 
        adj[v].push_back({u, wt}); 
    } 
    return adj; 
} 

vector<int> dijkstra(int n, vector<vector<int>> &edges, int src) { 
    vector<vector<pair<int, int>>> adj = constAdj(edges, n); 
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, 
    int>>> pq; 
    vector<int> dist(n, INT_MAX); 
    pq.push({0, src}); 
    dist[src] = 0; 

    while (!pq.empty()) { 
        int u = pq.top().second; 
        int d = pq.top().first; 
        pq.pop(); 

        if (d > dist[u]) continue; 

        for (auto &x : adj[u]) { 
            int v = x.first, wt = x.second; 
            if (dist[u] + wt < dist[v]) { 
                dist[v] = dist[u] + wt; 
                pq.push({dist[v], v}); 
            } 
        } 
    } 
    return dist; 
} 

int main() { 
    int n, num_edges; 
    cout << "Enter number of vertices: "; 
    cin >> n; 

    cout << "Enter number of edges: "; 
    cin >> num_edges; 

    vector<vector<int>> edges(num_edges, vector<int>(3)); 
    cout << "Enter edges list (u v wt for each edge):" << endl; 
    for (int i = 0; i < num_edges; i++) { 
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2]; 
    } 

    int src; 
    cout << "Enter source vertex: "; 
    cin >> src; 

    vector<int> result = dijkstra(n, edges, src); 

    cout << "Shortest distances from source " << src << " :" << endl; 
    for (int i = 0; i < n; i++) { 
        cout << "Vertex " << i << " -> " << result[i] << endl; 
    } 

    return 0; 
}
