#include <bits/stdc++.h>

using namespace std;

int invalid = 10000009;

pair<int, int> find_start(vector<string> board) {
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[i].size(); j++) {
            if(board[i][j] == 'S') {
                return make_pair(i, j);
            }
        }
    }
    return make_pair(-1, -1);
}

pair<int, int> find_goal(vector<string> board) {
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[i].size(); j++) {
            if(board[i][j] == 'G') {
                return make_pair(i, j);
            }
        }
    }
    return make_pair(-1, -1);
}


bool in_bounds(pair<int, int> loc, int h, int w) {
    if(loc.first < 0) return false;
    if(loc.second < 0) return false;
    if(loc.first >= h) return false;
    if(loc.second >= w) return false;
    return true;
}

int find_father_path(int len, vector<string> board, int height, int width) {
    auto start = find_start(board);
    auto goal = find_goal(board);

    //we have a copy of the board. when stepping off of a tile, set it to 'B' to mark it as used.

    int steps = 0;
    vector<pair<int, int>> locs, future_locs;
    locs.push_back(start);
    board[start.first][start.second] = 'B';

    while(locs.size()) {
        //per loc, find moves, make moves, set them to 'B'
        for(auto loc : locs) {
            if(loc == goal) {
                return steps;
            }
            //we can move in the cardinal directions, up to len squares.
            for(int l = 1; l <= len; l++) {
                auto new_loc = make_pair(loc.first - l, loc.second);
                if(in_bounds(new_loc, height, width)) {
                    if(new_loc == goal) {
                        return steps + 1;
                    }

                    //maybe can move here
                    if(board[new_loc.first][new_loc.second] != 'B') {
                        future_locs.push_back(new_loc);
                        board[new_loc.first][new_loc.second] = 'B';
                    }
                }

                new_loc = make_pair(loc.first + l, loc.second);
                if(in_bounds(new_loc, height, width)) {
                    if(new_loc == goal) {
                        return steps + 1;
                    }

                    //maybe can move here
                    if(board[new_loc.first][new_loc.second] != 'B') {
                        future_locs.push_back(new_loc);
                        board[new_loc.first][new_loc.second] = 'B';
                    }
                }
                new_loc = make_pair(loc.first, loc.second - l);
                if(in_bounds(new_loc, height, width)) {
                    if(new_loc == goal) {
                        return steps + 1;
                    }

                    //maybe can move here
                    if(board[new_loc.first][new_loc.second] != 'B') {
                        future_locs.push_back(new_loc);
                        board[new_loc.first][new_loc.second] = 'B';
                    }
                }
                new_loc = make_pair(loc.first, loc.second + l);
                if(in_bounds(new_loc, height, width)) {
                    if(new_loc == goal) {
                        return steps + 1;
                    }

                    //maybe can move here
                    if(board[new_loc.first][new_loc.second] != 'B') {
                        future_locs.push_back(new_loc);
                        board[new_loc.first][new_loc.second] = 'B';
                    }
                }
            }
        }

        //then swap locs and future locs and clear them.
        steps++;
        locs.swap(future_locs);
        future_locs.clear();
    }
    return invalid;
}

int find_child_path(int len, vector<string> board, int height, int width) {
        auto start = find_start(board);
    auto goal = find_goal(board);

    //we have a copy of the board. when stepping off of a tile, set it to 'B' to mark it as used.

    int steps = 0;
    vector<pair<int, int>> locs, future_locs;
    locs.push_back(start);
    board[start.first][start.second] = 'B';

    while(locs.size()) {
        //per loc, find moves, make moves, set them to 'B'
        for(auto loc : locs) {
            if(loc == goal) {
                return steps;
            }
            //we can move in the cardinal directions, up to len squares.
            for(int lx = -len; lx <= len; lx++) {
                for (int ly = -len; ly <= len; ly++) {
                    auto new_loc = make_pair(loc.first + lx, loc.second + ly);
                    if(in_bounds(new_loc, height, width) && ((lx * lx) + (ly * ly) <= (len * len))) {
                        if(new_loc == goal) {
                            return steps + 1;
                        }

                        //maybe can move here
                        if(board[new_loc.first][new_loc.second] != 'B') {
                            future_locs.push_back(new_loc);
                            board[new_loc.first][new_loc.second] = 'B';
                        }
                    }
                }
            }
        }

        //then swap locs and future locs and clear them.
        steps++;
        locs.swap(future_locs);
        future_locs.clear();
    }
    return invalid;
}

int main() {
    int a, f, l, w;
    char newline;
    cin >> a >> f >> l >> w;

    vector<string> board(l);
    for(int i = 0; i < l; i++) {
        cin >> board[i];
    }

    int steps_father = find_father_path(f, board, l, w);

    cerr << steps_father << endl;

    int steps_child = find_child_path(a, board, l, w);

    cerr << steps_child << endl;

    if(steps_child == invalid && steps_father == invalid) {
        cout << "NO WAY" << endl;
    } else if(steps_child == steps_father) {
        cout << "SUCCESS" << endl;
    } else if(steps_child < steps_father) {
        cout << "GO FOR IT" << endl;
    } else {
        cout << "NO CHANCE" << endl;
    }
}
