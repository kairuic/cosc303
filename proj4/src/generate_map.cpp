// Project 4: generate_map.cpp
// Name: Kerry Cao

// Generates a random N x N tile map in the dijkstras input format, with 
// the start at the top-left corner and the end at the bottom-right corner.

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Main Execution

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " N" << endl;
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        cerr << "N must be a positive integer" << endl;
        return 1;
    }

    srand(time(NULL));

    char tiles[6] = {'f', 'g', 'G', 'h', 'm', 'r'};
    int costs[6] = {3, 1, 2, 4, 7, 5};

    // tile types and costs
    cout << 6 << "\n";
    for (int i = 0; i < 6; i++) {
        cout << tiles[i] << " " << costs[i] << "\n";
    }

    // random map
    cout << n << " " << n << "\n";
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            cout << tiles[rand() % 6];
            if (c < n - 1) cout << " ";
        }
        cout << "\n";
    }

    // start top-left, end bottom-right
    cout << 0 << " " << 0 << "\n";
    cout << n - 1 << " " << n - 1 << "\n";

    return 0;
}