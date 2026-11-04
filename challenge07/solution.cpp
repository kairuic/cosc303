// Challenge 7: Sequence Alignment
// Name: Kerry Cao
// Overview: Computes the optimal global-alignment score (Needleman-Wunsch)
// for a pair of sequences read from standard input and prints D[m][n].
// Scoring: match = +1, mismatch = -1, gap = -1.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

const int MATCH = 1;
const int MISMATCH = -1;
const int GAP = -1;

int main() {
  vector<string> seqs;
  string line;
  bool fasta = false;

  while (getline(cin, line)) {
    if (!line.empty() && line.back() == '\r') line.pop_back();  // Windows line endings

    if (!line.empty() && line[0] == '>') {
      // FASTA header starts a new sequence
      fasta = true;
      seqs.push_back("");
    } else if (fasta) {
      for (char c : line)
        if (!isspace((unsigned char)c)) seqs.back() += toupper(c);
    } else {
      // Plain format: each word is a sequence
      string tok;
      for (char c : line + " ") {
        if (isspace((unsigned char)c)) {
          if (!tok.empty()) seqs.push_back(tok);
          tok.clear();
        } else {
          tok += toupper(c);
        }
      }
    }
  }

  if (seqs.size() < 2) {
    cerr << "Error: expected two sequences\n";
    return (1);
  }

  string s1 = seqs[0], s2 = seqs[1];
  int m = s1.size(), n = s2.size();

  // D[i][j] = best score aligning first i chars of s1 with first j of s2
  vector<vector<int>> D(m + 1, vector<int>(n + 1, 0));

  // Base cases: first row and column are all gaps
  for (int i = 1; i <= m; i++) D[i][0] = D[i - 1][0] + GAP;
  for (int j = 1; j <= n; j++) D[0][j] = D[0][j - 1] + GAP;

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      // Diagonal: match or mismatch
      int diag;
      if (s1[i - 1] == s2[j - 1])
        diag = D[i - 1][j - 1] + MATCH;
      else
        diag = D[i - 1][j - 1] + MISMATCH;

      int up = D[i - 1][j] + GAP;    // Gap in s2
      int left = D[i][j - 1] + GAP;  // Gap in s1

      D[i][j] = max({diag, up, left});
    }
  }

  cout << "Alignment score: " << D[m][n] << "\n";
  return (0);
}