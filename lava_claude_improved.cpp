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

int find_path(int max_dist, vector<string> board, bool is_father) {
    auto start = find_pos(board, 'S');
    auto goal = find_pos(board, 'G');
    
    int steps = 0;
    vector<pair<int, int>> current = {start};
    board[start.first][start.second] = 'B';
    
    while (!current.empty()) {
        vector<pair<int, int>> next;
        
        for (auto [x, y] : current) {
            if (make_pair(x, y) == goal) return steps;
            
            if (is_father) {
                // Father moves in cardinal directions only
                int dx[] = {-1, 1, 0, 0};
                int dy[] = {0, 0, -1, 1};
                
                for (int dir = 0; dir < 4; dir++) {
                    for (int dist = 1; dist <= max_dist; dist++) {
                        int nx = x + dx[dir] * dist;
                        int ny = y + dy[dir] * dist;
                        
                        if (!in_bounds(nx, ny, board.size(), board[0].size())) break;
                        if (make_pair(nx, ny) == goal) return steps + 1;
                        
                        if (board[nx][ny] != 'B') {
                            next.push_back({nx, ny});
                            board[nx][ny] = 'B';
                        }
                    }
                }
            } else {
                // Child moves in a circle
                for (int dx = -max_dist; dx <= max_dist; dx++) {
                    for (int dy = -max_dist; dy <= max_dist; dy++) {
                        if (dx*dx + dy*dy > max_dist*max_dist) continue;
                        
                        int nx = x + dx, ny = y + dy;
                        if (!in_bounds(nx, ny, board.size(), board[0].size())) continue;
                        if (make_pair(nx, ny) == goal) return steps + 1;
                        
                        if (board[nx][ny] != 'B') {
                            next.push_back({nx, ny});
                            board[nx][ny] = 'B';
                        }
                    }
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
    
    int steps_father = find_path(f, board, true);
    int steps_child = find_path(a, board, false);
    
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
