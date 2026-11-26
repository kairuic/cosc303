// Project 5: worddice.cpp
// Name: Kerry Cao

// Reads a list of dice and a list of words, then checks if each word can be
// spelled using each die at most once. It builds a flow network for each word
// and uses Edmonds-Karp to find the max flow. It then prints which die is used
// for each letter, or "Cannot spell" if the word can't be made.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// one edge in the residual graph
struct Edge {
    int to; // node this edge goes to
    int cap; // capacity left on this edge
    int rev; // index of the reverse edge in adj[to]
    bool original; // true if real edge, false if reverse edge
};

class Graph {
public:
    vector<vector<Edge>> adj;

    // clear the graph and make n empty nodes
    void reset(int n) {
        adj.assign(n, vector<Edge>());
    }

    // add edge u -> v and its reverse edge v -> u with 0 capacity
    void addEdge(int u, int v, int c) {
        adj[u].push_back({v, c, (int)adj[v].size(), true});
        adj[v].push_back({u, 0, (int)adj[u].size() - 1, false});
    }

    // Edmonds-Karp: use BFS to find paths from s to t until none are left
    int maxFlow(int s, int t) {
        int flow = 0;
        int n = adj.size();
        vector<int> prevNode(n), prevEdge(n);

        while (true) {
            fill(prevNode.begin(), prevNode.end(), -1);
            prevNode[s] = s;
            queue<int> q;
            q.push(s);

            // BFS on edges that still have capacity
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

            // no path to sink, so we are done
            if (prevNode[t] == -1) break;

            // push 1 unit of flow back along the path
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

// Main Execution

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "usage: worddice dice-file word-file" << endl;
        return 1;
    }

    ifstream diceFile(argv[1]), wordFile(argv[2]);
    if (!diceFile || !wordFile) {
        cerr << "Could not open input files" << endl;
        return 1;
    }

    // read in the dice
    vector<string> dice;
    string line;
    while (diceFile >> line) dice.push_back(line);

    // hasLetter[d][c] is true if die d has letter c
    vector<vector<bool>> hasLetter(dice.size(), vector<bool>(256, false));
    for (size_t d = 0; d < dice.size(); d++)
        for (unsigned char c : dice[d]) hasLetter[d][c] = true;

    int nd = dice.size();
    Graph g;

    while (wordFile >> line) {
        int nl = line.size();

        // nodes: 0 = source, 1..nd = dice, nd+1..nd+nl = letters, last = sink
        int source = 0, sink = nd + nl + 1;
        g.reset(nd + nl + 2);

        // source -> each die
        for (int d = 0; d < nd; d++) g.addEdge(source, 1 + d, 1);

        // die -> letter if the die has that letter
        for (int d = 0; d < nd; d++)
            for (int i = 0; i < nl; i++)
                if (hasLetter[d][(unsigned char)line[i]])
                    g.addEdge(1 + d, nd + 1 + i, 1);

        // each letter -> sink
        for (int i = 0; i < nl; i++) g.addEdge(nd + 1 + i, sink, 1);

        // word can only be spelled if every letter gets a die
        if (g.maxFlow(source, sink) != nl) {
            cout << "Cannot spell " << line << endl;
            continue;
        }

        // a die -> letter edge with 0 capacity left is being used
        vector<int> assign(nl, -1);
        for (int d = 0; d < nd; d++)
            for (const Edge &e : g.adj[1 + d])
                if (e.original && e.to > nd && e.to < sink && e.cap == 0)
                    assign[e.to - nd - 1] = d;

        // print die used for each letter, then the word
        for (int i = 0; i < nl; i++) cout << (i ? "," : "") << assign[i];
        cout << ": " << line << endl;
    }

    return 0;
}