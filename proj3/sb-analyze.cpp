//Project 3A: Superball Analyze
//Name: Kerry Cao

//Reads in a Superball board and prints every
//component that can be scored.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <vector>

#include "disjoint.h"

using namespace std;

class Superball {
  public:
    Superball(int argc, char **argv);
    void analyze_superball();
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
};

void usage(const char *s)
{
  fprintf(stderr, "usage: sb-analyze rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < (int) strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if ((int) s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

//Finds and prints all of the scoring sets on the board

void Superball::analyze_superball()
{
  DisjointSetByRankWPC ds(r*c);
  vector <int> size(r*c, 0);        // size of each set, indexed by its root
  vector <int> scorecell(r*c, -1);  // a goal cell in each set (-1 if none)
  vector <int> printed(r*c, 0);     // so each set only gets printed once
  int i, j, k, s1, s2;

  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      k = i*c+j;
      if (board[k] == '.' || board[k] == '*') continue;

      //union with the right neighbor if it's the same color
      if (j+1 < c && board[k+1] == board[k]) {
        s1 = ds.Find(k);
        s2 = ds.Find(k+1);
        if (s1 != s2) ds.Union(s1, s2);
      }

      //union with the neighbor below if it's the same color
      if (i+1 < r && board[k+c] == board[k]) {
        s1 = ds.Find(k);
        s2 = ds.Find(k+c);
        if (s1 != s2) ds.Union(s1, s2);
      }
    }
  }

  //count the size of each set and save a goal cell for it
  for (k = 0; k < r*c; k++) {
    if (board[k] == '.' || board[k] == '*') continue;
    s1 = ds.Find(k);
    size[s1]++;
    if (goals[k] && scorecell[s1] == -1) scorecell[s1] = k;
  }

  //print every set that's big enough and touches a goal cell
  printf("Scoring sets:\n");
  for (k = 0; k < r*c; k++) {
    if (board[k] == '.' || board[k] == '*') continue;
    s1 = ds.Find(k);
    if (printed[s1]) continue;
    if (size[s1] >= mss && scorecell[s1] != -1) {
      printf("  Size: %2d  Char: %c  Scoring Cell: %d,%d\n",
             size[s1], board[k], scorecell[s1] / c, scorecell[s1] % c);
    }
    printed[s1] = 1;
  }
}

//Main Execution

int main(int argc, char **argv)
{
  Superball *s;

  s = new Superball(argc, argv);
  s->analyze_superball();
  delete s;
  return 0;
}