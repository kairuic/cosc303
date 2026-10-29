// Project 4: dijkstras.cpp
// Name: Kerry Cao

// Reads a tile map, its tile costs, and start/end positions then uses
// Dijkstra's algorithm to find the minimum-cost path. It then prints the total
// Path from start to end, followed by each cell on the path.

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// Main Execution

int main(int argc, char *argv[]) {
    int numTiles;
    cin >> numTiles;

    // tile symbol -> cost
    map<char, int> tileCost;
    for (int i = 0; i < numTiles; i++) {
        char name;
        int cost;
        cin >> name >> cost;
        tileCost[name] = cost;
    }

    int rows, cols;
    cin >> rows >> cols;

    // cost of each cell
    vector<vector<int>> grid(rows, vector<int>(cols));
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            char t;
            cin >> t;
            grid[r][c] = tileCost[t];
        }
    }

    int startR, startC, endR, endC;
    cin >> startR >> startC >> endR >> endC;

    // each cell is a vertex: id = row * cols + col
    int n = rows * cols;
    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    vector<bool> visited(n, false);

    // min heap of (distance, vertex)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    int start = startR * cols + startC;
    int end = endR * cols + endC;
    dist[start] = 0;
    pq.push(make_pair(0, start));

    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        int r = u / cols;
        int c = u % cols;

        // check up, down, left, right
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;

            int v = nr * cols + nc;
            // cost of leaving the current tile
            int newDist = dist[u] + grid[r][c];
            if (newDist < dist[v]) {
                dist[v] = newDist;
                prev[v] = u;
                pq.push(make_pair(newDist, v));
            }
        }
    }

    // rebuild path from end back to start
    vector<int> path;
    for (int v = end; v != -1; v = prev[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    cout << dist[end] << endl;
    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i] / cols << " " << path[i] % cols << endl;
    }

    return 0;
}