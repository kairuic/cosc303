// Project 3B: Superball Play
// Name: Kerry Cao

//Reads in a Superball board and prints one move
//SWAP or SCORE for sb-player to make.

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
    void play();
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;

  protected:
    void components(const vector <int> &b, vector <int> &rep, vector <int> &size,
                    vector <int> &hasgoal);
    double evaluate(const vector <int> &b);
};

void usage(const char *s)
{
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
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

static inline bool sb_empty(int ch) { return ch == '.' || ch == '*'; }

//Finds the connected groups on board b. rep[k] is the root of cell k's set,
//and size[] and hasgoal[] are indexed by that root.

void Superball::components(const vector <int> &b, vector <int> &rep, vector <int> &size,
                           vector <int> &hasgoal)
{
  DisjointSetByRankWPC ds(r*c);
  int i, j, k, s1, s2;

  for (i = 0; i < r; i++) {
    for (j = 0; j < c; j++) {
      k = i*c+j;
      if (sb_empty(b[k])) continue;
      if (j+1 < c && b[k+1] == b[k]) {
        s1 = ds.Find(k); s2 = ds.Find(k+1);
        if (s1 != s2) ds.Union(s1, s2);
      }
      if (i+1 < r && b[k+c] == b[k]) {
        s1 = ds.Find(k); s2 = ds.Find(k+c);
        if (s1 != s2) ds.Union(s1, s2);
      }
    }
  }

  rep.assign(r*c, -1);
  size.assign(r*c, 0);
  hasgoal.assign(r*c, 0);
  for (k = 0; k < r*c; k++) {
    if (sb_empty(b[k])) continue;
    rep[k] = ds.Find(k);
    size[rep[k]]++;
    if (goals[k]) hasgoal[rep[k]] = 1;
  }
}

//Gives a board a score so I can compare swaps. Bigger groups of better
//colors are worth more.

double Superball::evaluate(const vector <int> &b)
{
  vector <int> rep, size, hasgoal;
  double v = 0;
  int k, s;

  components(b, rep, size, hasgoal);

  for (k = 0; k < r*c; k++) {
    if (rep[k] != k) continue; //only count each set once
    s = size[k];
    if (s < 2) continue;
    v += (double) s * s * colors[b[k]];
  }
  return v;
}

void Superball::play()
{
  vector <int> rep, size, hasgoal;
  int k, i, j;
  int bestscore = -1, bestcell = -1;

  //find the best set I can score right now

  components(board, rep, size, hasgoal);
  for (k = 0; k < r*c; k++) {
    if (sb_empty(board[k]) || !goals[k]) continue;
    int root = rep[k];
    if (size[root] >= mss) {
      int pts = size[root] * colors[board[k]];
      if (pts > bestscore) { bestscore = pts; bestcell = k; }
    }
  }

  //score it if I can, otherwise look for a swap

  if (bestcell != -1) {
    printf("SCORE %d %d\n", bestcell / c, bestcell % c);
    return;
  }

  //try every swap of two different colored pieces and keep the best one

  vector <int> b = board;
  double bestval = -1;
  int bi = -1, bj = -1;

  for (i = 0; i < r*c; i++) {
    if (sb_empty(b[i])) continue;
    for (j = i+1; j < r*c; j++) {
      if (sb_empty(b[j]) || b[i] == b[j]) continue;
      swap(b[i], b[j]);
      double v = evaluate(b);
      swap(b[i], b[j]);
      if (v > bestval) { bestval = v; bi = i; bj = j; }
    }
  }


  //no good move left, so just swap any two cells so the game can end

  if (bi == -1) {
    bi = bj = -1;
    for (k = 0; k < r*c; k++) {
      if (!sb_empty(board[k])) {
        if (bi == -1) bi = k; else if (bj == -1) bj = k;
      }
    }
    if (bi == -1) bi = 0;
    if (bj == -1) bj = (bi == 0) ? 1 : 0;
  }

  printf("SWAP %d %d %d %d\n", bi / c, bi % c, bj / c, bj % c);
}

//Main Execution

int main(int argc, char **argv)
{
  Superball *s;

  s = new Superball(argc, argv);
  s->play();
  delete s;
  return 0;
}