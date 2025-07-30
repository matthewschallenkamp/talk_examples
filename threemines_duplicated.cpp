#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <cctype>
#include <algorithm>

using namespace std;

// Written in ~1h by Matthew, borrowing some code from simple_dp.cpp. Did take a little debugging to get the indexing exactly right.
// Don't edit this further - we want this to be representative of the best we can see at this level.
// If you want to make cleanups/improvements they can go into fast_dp_cleaned.cpp or similar.
// This should have performance of O(N^3), with an added log(N) and some constant overhead from the map memo instead of an explicit table.

int neg_inf = INT_MIN / 4;

struct rectangle {
  // inclusive/exclusive indexing
  int low_x, low_y, high_x, high_y;
};

bool operator<(rectangle l, rectangle r) {
  // what make_pair is obscuring here is we just compare all the values in order.
  auto lp = make_pair(make_pair(l.low_x, l.low_y), make_pair(l.high_x, l.high_y));
  auto rp = make_pair(make_pair(r.low_x, r.low_y), make_pair(r.high_x, r.high_y));
  return lp < rp;
}


int rectangle_sum(vector<vector<int> > &cumulative_sum, rectangle r) {
  // Like how you can get a linear range sum from a cumulative sum in one dimension, 
  // we can get a rectangular range sum from a cumulative sum in two dimensions. 
  // ....|....|....
  // ----A----B....
  // ....|....|....
  // ....|....|....
  // ----C----D....
  // ..............
  // We'll add the value at point D so we cover the whole rectangle area, 
  // subtract out the values at B and C so that we don't include those sections that aren't part of the rectangle
  // and then re-add the value at A because we subtracted it out twice, to get the exact rectangle.

  int sum = cumulative_sum[r.high_x][r.high_y] 
    - cumulative_sum[r.high_x][r.low_y] 
    - cumulative_sum[r.low_x][r.high_y] 
    + cumulative_sum[r.low_x][r.low_y];
  return sum;
}

// Key idea:
// For <=3 rectangles on a grid, there's always a way to draw a line that splits them. 
// Figuring out the possibilities for how that divides the grid, there's only ~3 different possibilities:
// We can have 3 different column blocks, 3 different row blocks, or one column/row block and two other corner blocks.

//  ________      ________      _______
// |  |  |  |    |________|    |   |   |
// |  |  |  |    |        |    |___|   |
// |  |  |  |    |________|    |   |   |
// |__|__|__|    |________|    |___|___|

// So, knowing the shapes that arise, we never need to figure out the maximal rectangle in some interior shape
// We can always have some outside edge on our queries, which means that we can calculate them in N^3 with some DP. 
// We'll have 4 auxiliary grids for the solutions with the different outside edges fixed 
// Actually 3 - we don't ever need the bottom edge fixed, we can just use left and right to cover.
// To make these we'll recurse on tightening each of the 2 parallel edges to get less-than-full width rectangles.
// Then for full-width rectangles we can treat it like a 1d array and use Kadane's algorithm.

// Indexes are (lesser parallel edge, greater parallel edge, perpindicular edge)
vector<vector<vector<int> > > left_fixed, right_fixed, up_fixed;

void fill_best_rectangle_in_section(vector<vector<int> > &cumulative_sum, int vmax, int hmax) {

  // up
  // initialize to width by width by height, fill with negative infinity.
  up_fixed.resize(hmax + 1, vector<vector<int> >(hmax + 1, vector<int>(vmax + 1, neg_inf)));

  for(int len = 1; len <= hmax; len++) {
    for(int low_x = 0; low_x + len <= hmax; low_x++) {
      int high_x = low_x + len;

      vector<int> &col = up_fixed[low_x][high_x];
      int current = 0, best = neg_inf;
      for(int i = 1; i <= vmax; i++) {
        col[i] = max(up_fixed[low_x + 1][high_x][i], up_fixed[low_x][high_x - 1][i]);

        current += rectangle_sum(cumulative_sum, rectangle{low_x, i-1, high_x, i});
        best = max(best, current);
        col[i] = max(col[i], best);
        current = max(current, 0);
      }
    }
  }


  // left_fixed
  // initialize to height by height by width, fill with negative infinity.
  left_fixed.resize(vmax + 1, vector<vector<int> >(vmax + 1, vector<int>(hmax + 1, neg_inf)));

  for(int len = 1; len <= vmax; len++) {
    for(int low_y = 0; low_y + len <= vmax; low_y++) {
      int high_y = low_y + len;

      vector<int> &col = left_fixed[low_y][high_y];
      int current = 0, best = neg_inf;
      for(int i = 1; i <= hmax; i++) {
        col[i] = max(left_fixed[low_y + 1][high_y][i], left_fixed[low_y][high_y - 1][i]);

        current += rectangle_sum(cumulative_sum, rectangle{i-1, low_y, i, high_y});
        best = max(best, current);
        col[i] = max(col[i], best);
        current = max(current, 0);
      }
    }
  }

  // right_fixed
  // initialize to height by height by width, fill with negative infinity.
  right_fixed.resize(vmax + 1, vector<vector<int> >(vmax + 1, vector<int>(hmax + 1, neg_inf)));

  for(int len = 1; len <= vmax; len++) {
    for(int low_y = 0; low_y + len <= vmax; low_y++) {
      int high_y = low_y + len;

      vector<int> &col = right_fixed[low_y][high_y];
      int current = 0, best = neg_inf;
      // slightly different indexing than up and left so we keep the nice inclusive/exclusive property that 
      // makes our combination logic not need any +-1's later.
      for(int i = hmax-1; i >= 0; i--) {
        col[i] = max(right_fixed[low_y + 1][high_y][i], right_fixed[low_y][high_y - 1][i]);

        current += rectangle_sum(cumulative_sum, rectangle{i, low_y, i+1, high_y});
        best = max(best, current);
        col[i] = max(col[i], best);
        current = max(current, 0);
      }
    }
  }
}


int solve(const vector<vector<int> > &grid) {
  int vmax = grid[0].size();
  int hmax = grid.size();


  // Note when we're making our cumulative sum table that we make it larger than the original grid
  // and sort of shift it out one index in both directions.
  // This makes our indexing easier later - We can just use inclusive/exclusive indexing for the rectangles and 
  // the rectangle sum function doesn't have to do any off-by-one sort of tweaks.
  vector<vector<int > > cumulative_sum(hmax + 1);
  cumulative_sum[0].resize(vmax + 1, 0);

  for(int i = 0; i < hmax; i++) {
    int row_sum = 0;
    cumulative_sum[i + 1].resize(vmax + 1, 0);
    for(int j = 0; j < vmax; j++) {
      row_sum += grid[i][j];
      cumulative_sum[i+1][j+1] = cumulative_sum[i][j+1] + row_sum;
    }
  }

  fill_best_rectangle_in_section(cumulative_sum, vmax, hmax);

  int best = neg_inf;

  // two vertical lines
  for(int i = 1; i < hmax; i++) {
    for(int j = i+1; j < hmax; j++) {
      best = max(best, up_fixed[0][i][vmax] + up_fixed[i][j][vmax] + up_fixed[j][hmax][vmax]);
    }
  }

  // two horizontal lines
  for(int i = 1; i < vmax; i++) {
    for(int j = i+1; j < vmax; j++) {
      best = max(best, left_fixed[0][i][hmax] + left_fixed[i][j][hmax] + left_fixed[j][vmax][hmax]);
    }
  }

  // mixed lines
  for(int horizontal = 1; horizontal < hmax; horizontal++) {
    for(int vertical = 1; vertical < vmax; vertical++) {
      int next;

      // four possiblities here. The edges always make a cross -|- and we can ignore one of the 4 parts of that cross.
      // we also have to take care to make sure that our indexes don't overlap, hence the -1 and +1s.
      // up
      next = left_fixed[0][vertical][hmax] + left_fixed[vertical][vmax][horizontal] + right_fixed[vertical][vmax][horizontal];
      best = max(best, next);
      // down
      next = left_fixed[vertical][vmax][hmax] + left_fixed[0][vertical][horizontal] + right_fixed[0][vertical][horizontal];
      best = max(best, next);

      // left
      next = left_fixed[0][vmax][horizontal] + right_fixed[0][vertical][horizontal] + right_fixed[vertical][vmax][horizontal];
      best = max(best, next);
      // right
      next = right_fixed[0][vmax][horizontal] + left_fixed[0][vertical][horizontal] + left_fixed[vertical][vmax][horizontal];
      best = max(best, next);
    }
  }

  return best;
}


int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int> > grid(n);
  for(int i = 0; i < grid.size(); i++) {
    grid[i].resize(m);
    for(int j = 0; j < grid[i].size(); j++) {
      cin >> grid[i][j];
    }
  }

  int best = solve(grid);
  cout << best << endl;
}
