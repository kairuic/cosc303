// Name: Kerry Cao
// Assignment: CS303 Project 5 -- Letter Dice (worddice)
// Overview: For each word, builds a flow network SOURCE -> dice -> letter positions -> SINK
//           (all capacities 1) and runs Edmonds-Karp (BFS augmenting paths on the residual
//           graph). If max flow == word length, the die->letter edges carrying flow give the
//           assignment; otherwise the word cannot be spelled.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct Edge {
    int to;        // destination node
    int cap;       // residual capacity
    int rev;       // index of the reverse edge in adj[to]
    bool original; // true for real edges, false for reverse (residual) edges
};

class Graph {
public:
    vector<vector<Edge>> adj;

    void reset(int n) {
        adj.assign(n, vector<Edge>());
    }

    // Adds edge u->v with capacity c plus its zero-capacity reverse edge v->u.
    void addEdge(int u, int v, int c) {
        adj[u].push_back({v, c, (int)adj[v].size(), true});
        adj[v].push_back({u, 0, (int)adj[u].size() - 1, false});
    }

    // Edmonds-Karp: repeatedly find the shortest augmenting path with BFS.
    int maxFlow(int s, int t) {
        int flow = 0;
        int n = adj.size();
        vector<int> prevNode(n), prevEdge(n);

        while (true) {
            fill(prevNode.begin(), prevNode.end(), -1);
            prevNode[s] = s;
            queue<int> q;
            q.push(s);

            while (!q.empty() && prevNode[t] == -1) {
                int u = q.front();
                q.pop();
                for (int i = 0; i < (int)adj[u].size(); i++) {
                    Edge &e = adj[u][i];
                    if (e.cap > 0 && prevNode[e.to] == -1) {
                        prevNode[e.to] = u;
                        prevEdge[e.to] = i;
                        q.push(e.to);
                    }
                }
            }

            if (prevNode[t] == -1) break; // no augmenting path left

            // All capacities are 1, so the bottleneck is always 1.
            for (int v = t; v != s; v = prevNode[v]) {
                Edge &e = adj[prevNode[v]][prevEdge[v]];
                e.cap -= 1;
                adj[v][e.rev].cap += 1;
            }
            flow++;
        }
        return flow;
    }
};

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "usage: worddice dice-file word-file" << endl;
        return 1;
    }

    ifstream diceFile(argv[1]), wordFile(argv[2]);
    if (!diceFile || !wordFile) {
        cerr << "Could not open input files" << endl;
        return 1;
    }

    vector<string> dice;
    string line;
    while (diceFile >> line) dice.push_back(line);

    // Precompute which letters each die has (word independent).
    vector<vector<bool>> hasLetter(dice.size(), vector<bool>(256, false));
    for (size_t d = 0; d < dice.size(); d++)
        for (unsigned char c : dice[d]) hasLetter[d][c] = true;

    int nd = dice.size();
    Graph g;

    while (wordFile >> line) {
        int nl = line.size();
        // Node layout: 0 = SOURCE, 1..nd = dice, nd+1..nd+nl = letters, nd+nl+1 = SINK
        int source = 0, sink = nd + nl + 1;
        g.reset(nd + nl + 2); // fresh graph each word: no flow carries over

        for (int d = 0; d < nd; d++) g.addEdge(source, 1 + d, 1);
        for (int d = 0; d < nd; d++)
            for (int i = 0; i < nl; i++)
                if (hasLetter[d][(unsigned char)line[i]]) // one edge even if die repeats a letter
                    g.addEdge(1 + d, nd + 1 + i, 1);
        for (int i = 0; i < nl; i++) g.addEdge(nd + 1 + i, sink, 1);

        if (g.maxFlow(source, sink) != nl) {
            cout << "Cannot spell " << line << endl;
            continue;
        }

        // A die->letter edge with cap 0 is carrying flow.
        vector<int> assign(nl, -1);
        for (int d = 0; d < nd; d++)
            for (const Edge &e : g.adj[1 + d])
                if (e.original && e.to > nd && e.to < sink && e.cap == 0)
                    assign[e.to - nd - 1] = d;

        for (int i = 0; i < nl; i++) cout << (i ? "," : "") << assign[i];
        cout << ": " << line << endl;
    }
    return 0;
}