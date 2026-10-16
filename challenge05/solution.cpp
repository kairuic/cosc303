// Challenge 5: Minimum Spanning Tree
// Name: Kerry Cao
// Overview: Computes a minimum spanning tree for each input graph using
// Prim's algorithm and prints its total weight and edges.

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

// Main Execution

int main(int argc, char *argv[]) {
  int n;
  bool firstGraph = true;

  while (cin >> n) {
    // Read the N x N adjacency matrix
    vector<vector<int>> adj(n, vector<int>(n));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        cin >> adj[i][j];

    // Prim's algorithm: key = cheapest edge to tree, parent = MST edge source
    vector<int> key(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<bool> inTree(n, false);

    key[0] = 0;  // Start growing the tree from vertex A
    int totalWeight = 0;

    for (int count = 0; count < n; count++) {
      // Select the non-tree vertex with the smallest key
      int u = -1;
      for (int v = 0; v < n; v++)
        if (!inTree[v] && (u == -1 || key[v] < key[u]))
          u = v;

      inTree[u] = true;
      totalWeight += key[u];

      // Update keys of non-tree neighbors through the new tree vertex u
      for (int v = 0; v < n; v++) {
        int w = adj[u][v];
        if (w != -1 && !inTree[v] && w < key[v]) {
          key[v] = w;
          parent[v] = u;
        }
      }
    }

    // Blank line between consecutive MSTs
    if (!firstGraph) cout << "\n";
    firstGraph = false;

    // Print total weight, then each tree edge with the earlier letter first
    // Collect tree edges (earlier letter first) and sort them alphabetically
    vector<string> edges;
    for (int v = 1; v < n; v++) {
      string e;
      e += (char)('A' + min(v, parent[v]));
      e += (char)('A' + max(v, parent[v]));
      edges.push_back(e);
    }
    sort(edges.begin(), edges.end());

    // Print total weight, then each edge
    cout << totalWeight << "\n";
    for (size_t i = 0; i < edges.size(); i++)
      cout << edges[i] << "\n";
  }
  return (0);
}