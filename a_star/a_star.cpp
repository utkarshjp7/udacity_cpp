#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
using std::vector;
using std::cout;
using std::string;


enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

string CellString(State state) {
    switch(state) {
        case State::kObstacle: return "⛰️   ";
        case State::kPath: return "🚗   ";
        case State::kStart: return "🚦   ";
        case State::kFinish: return "🏁   ";
        default: return "0   ";
    }
}

void Printgrid(vector<vector<State>> grid) {
    for (auto row: grid) {
        for (auto e: row) {
            std::cout << CellString(e);
        }
        std::cout << "\n";
    }
}

vector<State> ParseLine(string line) {
    std::istringstream line_stream(line);
    int n;
    char c;
    vector<State> grid_row;

    while (line_stream >> n >> c && c == ',') {
        if (n == 0) {
            grid_row.push_back(State::kEmpty);
        } else {
            grid_row.push_back(State::kObstacle);
        }
    }
    return grid_row;
}

vector<vector<State>> ReadgridFile(string path) {
    std::ifstream grid_file(path);
    string line;
    vector<vector<State>> grid;
    if (grid_file) {
        while (getline(grid_file, line)) {
            vector<State> row = ParseLine(line);
            grid.push_back(row);
        }
    }
    return grid;
}

int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>>& openNodes, vector<vector<State>>& grid) {
    vector<int> node = {x, y, g, h};
    openNodes.push_back(node);
    grid[x][y] = State::kClosed;
}

bool Compare(vector<int> node1, vector<int> node2) {
    int f1 = node1[2] + node1[3];
    int f2 = node2[2] + node2[3];
    return f1 > f2;
}

void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

bool CheckValidCell(int x, int y, vector<vector<State>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    if (x >= n || y >= m || x < 0 || y < 0) {
        return false;
    }
    if (grid[x][y] != State::kEmpty) {
        return false;
    }
    return true;
}

void ExpandNeighbors(vector<int>& currentNode, vector<vector<int>>& openNodes, vector<vector<State>>& grid, int goal[2]) {
    int x = currentNode[0];
    int y = currentNode[1];
    int g = currentNode[2];

    const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

    for (auto d: delta) {
        int x2 = x + d[0];
        int y2 = y + d[1];

        if (CheckValidCell(x2, y2, grid)) {
            int g2 = g+1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]); 
            AddToOpen(x2, y2, g2, h2, openNodes, grid);
        }
    }
}

vector<vector<State>> Search(vector<vector<State>> grid, int start[2], int goal[2]) {    
    vector<vector<int>> openNodes;
    int x = start[0];
    int y = start[1];
    int h = Heuristic(x, y, goal[0], goal[1]);
    int g = 0;

    AddToOpen(x, y, g, h, openNodes, grid);

    while (openNodes.size() > 0) {
        CellSort(&openNodes);
        vector<int> currentNode = openNodes.back();
        openNodes.pop_back();

        x = currentNode[0];
        y = currentNode[1];
        grid[x][y] = State::kPath;

        if (x == goal[0] && y == goal[1]) {
            grid[start[0]][start[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kFinish;
            return grid;
        }

        ExpandNeighbors(currentNode, openNodes, grid, goal);
    }

    return vector<vector<State>>();
}

int main() {
    vector<vector<State>> grid = ReadgridFile("1.grid");
    int start[2] = {0, 0};
    int goal[2] = {4, 5};

    vector<vector<State>> solution = Search(grid, start, goal);
    Printgrid(solution);
}