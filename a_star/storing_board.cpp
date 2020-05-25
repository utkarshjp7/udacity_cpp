#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using std::vector;
using std::string;


void PrintBoard(vector<vector<int>> board) {
    for (auto row: board) {
        for (auto e: row) {
            std::cout << e;
        }
        std::cout << "\n";
    }
}

vector<int> ParseLine(string line) {
    std::istringstream line_stream(line);
    int n;
    char c;
    vector<int> board_row;

    while (line_stream >> n >> c && c == ',') {
            board_row.push_back(n);
    }
    return board_row;
}

vector<vector<int>> ReadBoardFile(string path) {
    std::ifstream board_file(path);
    string line;
    vector<vector<int>> board;
    if (board_file) {
        while (getline(board_file, line)) {
            vector<int> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}

int main() { 
    vector<vector<int>> board = ReadBoardFile("1.grid");
    PrintBoard(board);
}
