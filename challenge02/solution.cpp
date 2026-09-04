// Challenge 02: Closest Numbers
// Name:  Kerry Cao 

// Brief description: Finds the closest pairs of numbers
// given a given list of integers.

// After given the total number of integers, and the list of integers
// The program sorts the now vector of integers then subtracting 
// the adjacent numbers, it finds the closest pairs of numbers 
// and outputs them.

// Resources: https://www.geeksforgeeks.org/cpp/difference-between-int-and-size_t-in-cpp/
// Wasn't compiling because I was comparing a unsigned int with a signed int
// On line 56

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

// Main Execution

int main(int argc, char *argv[]) {
    int n; // number of integers

    while (cin >> n) {

        vector<int> numbers(n);
        vector<pair<int, int>> closestPairs;

        // read the numbers from input
        for (int i = 0; i < n ; i++) {
            cin >> numbers[i];
        }

        // Sort the numbers
        sort(numbers.begin(), numbers.end());

        //For Loop to find the closest pairs
        int minDiff = INT_MAX;
        for (int i = 0; i < n - 1; i++) {
            int diff = numbers[i+1] - numbers [i];
            if (diff < minDiff) {
                minDiff = diff;
                closestPairs.clear();
                closestPairs.push_back(make_pair(numbers[i], numbers[i+1]));
            } else if (diff == minDiff) {
                closestPairs.push_back(make_pair(numbers[i], numbers[i+1]));
            }
        }

        // Output the closest pairs
        for (size_t i = 0; i < closestPairs.size(); i++) {
            cout << closestPairs[i].first << " " << closestPairs[i].second;
            
            // Add a space between pairs, but not after the last pair
            if (i < closestPairs.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
    return EXIT_SUCCESS;
}
