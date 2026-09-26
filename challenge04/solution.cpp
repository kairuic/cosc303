// Challenge 04: Graph Paths
// Name: Kerry Cao

// Brief description: Checks whether a path exists between two nodes
// in a directed graph.

// For each graph, I build an adjacency list by mapping each node to a
// vector of the nodes it points to. Then for every query I run a BFS
// starting from the source node and see if I ever reach the destination.
// I reset the visited set for every new query so old graphs don't mess
// with new ones.

#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Main Execution

int main(int argc, char *argv[]) {
  int graphNum = 0;
  int numEdges;

  while (cin >> numEdges) {
    if (graphNum > 0) {
      cout << endl; // blank line between graphs
    }
    graphNum++;

    // adjacency list: node -> list of nodes it has an edge to
    unordered_map<string, vector<string>> adj;

    for (int i = 0; i < numEdges; i++) {
      string src, dst;
      cin >> src >> dst;
      adj[src].push_back(dst);
    }

    int numQueries;
    cin >> numQueries;

    for (int q = 0; q < numQueries; q++) {
      string src, dst;
      cin >> src >> dst;

      bool found = false;

      // a node can always reach itself
      if (src == dst) {
        found = true;
      } else {
        // plain BFS from src
        unordered_set<string> visited;
        queue<string> toVisit;
        visited.insert(src);
        toVisit.push(src);

        while (!toVisit.empty() && !found) {
          string curr = toVisit.front();
          toVisit.pop();

          for (const string &next : adj[curr]) {
            if (next == dst) {
              found = true;
              break;
            }
            if (visited.find(next) == visited.end()) {
              visited.insert(next);
              toVisit.push(next);
            }
          }
        }
      }

      if (found) {
        cout << "In Graph " << graphNum << " there is a path from " << src
             << " to " << dst << endl;
      } else {
        cout << "In Graph " << graphNum << " there is no path from " << src
             << " to " << dst << endl;
      }
    }
  }

  return (0);
}