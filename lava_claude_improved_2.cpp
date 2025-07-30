#include <bits/stdc++.h>
using namespace std;

const int INVALID = 10000009;

pair<int, int> find_pos(const vector<string>& board, char target) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] == target) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

bool in_bounds(int x, int y, int h, int w) {
    return x >= 0 && y >= 0 && x < h && y < w;
}

int find_path(int max_dist, vector<string> board, function<vector<pair<int,int>>(int,int)> get_neighbors) {
    auto start = find_pos(board, 'S');
    auto goal = find_pos(board, 'G');
    
    int steps = 0;
    vector<pair<int, int>> current = {start};
    board[start.first][start.second] = 'B';
    
    while (!current.empty()) {
        vector<pair<int, int>> next;
        
        for (auto [x, y] : current) {
            if (make_pair(x, y) == goal) return steps;
            
            for (auto [nx, ny] : get_neighbors(x, y)) {
                if (!in_bounds(nx, ny, board.size(), board[0].size())) continue;
                if (make_pair(nx, ny) == goal) return steps + 1;
                
                if (board[nx][ny] != 'B') {
                    next.push_back({nx, ny});
                    board[nx][ny] = 'B';
                }
            }
        }
        
        current = move(next);
        steps++;
    }
    return INVALID;
}

int main() {
    int a, f, l, w;
    cin >> a >> f >> l >> w;
    
    vector<string> board(l);
    for (int i = 0; i < l; i++) {
        cin >> board[i];
    }
    
    // Father moves in cardinal directions
    auto father_neighbors = [f](int x, int y) {
        vector<pair<int, int>> neighbors;
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        
        for (int dir = 0; dir < 4; dir++) {
            for (int dist = 1; dist <= f; dist++) {
                neighbors.push_back({x + dx[dir] * dist, y + dy[dir] * dist});
            }
        }
        return neighbors;
    };
    
    // Child moves in a circle
    auto child_neighbors = [a](int x, int y) {
        vector<pair<int, int>> neighbors;
        for (int dx = -a; dx <= a; dx++) {
            for (int dy = -a; dy <= a; dy++) {
                if (dx*dx + dy*dy <= a*a && (dx != 0 || dy != 0)) {
                    neighbors.push_back({x + dx, y + dy});
                }
            }
        }
        return neighbors;
    };
    
    int steps_father = find_path(f, board, father_neighbors);
    int steps_child = find_path(a, board, child_neighbors);
    
    if (steps_child == INVALID && steps_father == INVALID) {
        cout << "NO WAY\n";
    } else if (steps_child == steps_father) {
        cout << "SUCCESS\n";
    } else if (steps_child < steps_father) {
        cout << "GO FOR IT\n";
    } else {
        cout << "NO CHANCE\n";
    }
}
