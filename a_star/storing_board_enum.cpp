#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using std::vector;
using std::string;

enum class State {kEmpty, kObstacle};

string CellString(State state) {
    switch(state) {
        case State::kObstacle: return "⛰️   ";
        default: return "0   ";
    }
}

void PrintBoard(vector<vector<State>> board) {
    for (auto row: board) {
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
    vector<State> board_row;

    while (line_stream >> n >> c && c == ',') {
        if (n == 0) {
            board_row.push_back(State::kEmpty);
        } else {
            board_row.push_back(State::kObstacle);
        }
    }
    return board_row;
}

vector<vector<State>> ReadBoardFile(string path) {
    std::ifstream board_file(path);
    string line;
    vector<vector<State>> board;
    if (board_file) {
        while (getline(board_file, line)) {
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}


int main() { 
    vector<vector<State>> board = ReadBoardFile("1.grid");
    PrintBoard(board);
}
