// Kerry Cao
// Aug 26
// Challenge 01: Rotating Arrays; null solution
// Given an array of integers, rotate the array to the left
// or right based on the input direction and number of rotations.

// I used Geek for Geeks for the toupper function and 
// #include <cctype> header file. 
// https://www.geeksforgeeks.org/cpp/toupper-in-cpp/


#include <iostream>
#include <cctype>

using namespace std;

int main(int argc, char *argv[]) {
    int n; // number of elements in the array
    int k; // number of rotations
    char r; // direction of rotation


while (cin >> n >> k >> r) {
    r = toupper(r); // Convert direction to uppercase 

    // cin the array elements
    int *arr = new int[n];
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Calculate the number of rotations
    switch (r) {
        case 'L':
            k = k % n;
            break;
        case 'R':
            k = n - (k % n);
            break;
        default:
            cout << "Invalid direction" << endl;
            return 1;
    }

    // Perform the rotation
    int *result = new int[n];
    for (int i = 0; i < n; i++) {
        int new_index = (i + k) % n;
        result[i] = arr[new_index];
    }
    

    // Print out the rotated array
    for (int i = 0; i < n; i++) {
        cout << result[i];
        
        // Match with output (last element should not have a space after it)
        if (i < n - 1) {
            cout << " ";
        }
    }

    cout << endl;

    delete[] arr;
    delete[] result;

}
    return 0;
}

