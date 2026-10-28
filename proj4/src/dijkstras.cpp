// dijsktras.cpp

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>
#include <algorithm>
#include <functional>
#include <utility>

using namespace std;

// Main Execution

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
 
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int tiles_n;
    if (!(cin >> tiles_n)) return 1;
 
    // Tile symbol -> cost lookup (indexed by char value)
    vector<int> tile_cost(256, -1);
    for (int i = 0; i < tiles_n; i++) {
        string name;
        int cost;
        cin >> name >> cost;
        tile_cost[(unsigned char)name[0]] = cost;
    }
 
    int rows, cols;
    cin >> rows >> cols;
 
    // Store each cell's cost directly; vertex id = r * cols + c
    int V = rows * cols;
    vector<int> cell_cost(V);
    for (int i = 0; i < V; i++) {
        string sym;
        cin >> sym;
        cell_cost[i] = tile_cost[(unsigned char)sym[0]];
    }
 
    int sr, sc, er, ec;
    cin >> sr >> sc >> er >> ec;
    int src = sr * cols + sc;
    int dst = er * cols + ec;
 
    const long long INF = numeric_limits<long long>::max();
    vector<long long> dist(V, INF);   // tentative distances
    vector<int> prev(V, -1);          // predecessor for path reconstruction
    vector<char> done(V, 0);          // finalized vertices
 
    // Min-heap of (distance, vertex); stale entries skipped (lazy deletion)
    using Item = pair<long long, int>;
    priority_queue<Item, vector<Item>, greater<Item>> pq;
 
    dist[src] = 0;
    pq.push({0, src});
 
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
 
    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (done[u]) continue;
        done[u] = 1;
        if (u == dst) break;   // destination finalized, no need to continue
 
        int r = u / cols, c = u % cols;
        long long w = cell_cost[u];  // cost of leaving u
 
        // Neighbors generated on the fly (no explicit edge list)
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            int v = nr * cols + nc;
            if (done[v]) continue;
            long long nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                prev[v] = u;
                pq.push({nd, v});
            }
        }
    }
 
    // Reconstruct path: walk predecessors from destination, then reverse
    vector<int> path;
    for (int v = dst; v != -1; v = prev[v]) path.push_back(v);
    reverse(path.begin(), path.end());
 
    string out;
    out.reserve(path.size() * 12 + 16);
    out += to_string(dist[dst]) + "\n";
    for (int v : path) {
        out += to_string(v / cols) + " " + to_string(v % cols) + "\n";
    }
    cout << out;
    return 0;
}

