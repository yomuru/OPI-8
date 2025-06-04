#include "game_logic.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>
using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

const string PARAMS_FILE = "params.txt";
const string STATS_FILE = "stats.txt";
const char EMPTY_CELL = '.';
const char X_MARK = 'X';
const char O_MARK = 'O';

void createFile(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        ofstream createFile(fileName);
        createFile.close();
    }
}

int loadSize() {
    ifstream file(PARAMS_FILE);
    int size = 3;
    if (file.is_open()) {
        file >> size;
        file.close();
    }
    return size;
}

pair<string, string> loadNames() {
    ifstream file(PARAMS_FILE);
    string p1 = "Игрок 1";
    string p2 = "Игрок 2";

    if (file.is_open()) {
        int tempSize;
        file >> tempSize;
        file.ignore();
        getline(file, p1);
        getline(file, p2);
        file.close();
    }

    return {p1, p2};
}

void saveParams(int size, const string& p1, const string& p2) {
    ofstream file(PARAMS_FILE);
    if (file.is_open()) {
        file << size << endl;
        file << p1 << endl;
        file << p2 << endl;
        file.close();
    }
}

vector<string> loadStats() {
    ifstream file(STATS_FILE);
    vector<string> stats;
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stats.push_back(line);
        }
        file.close();
    }
    return stats;
}

void saveStat(const string& record) {
    ofstream file(STATS_FILE, ios::app);
    if (file.is_open()) {
        file << record << endl;
        file.close();
    }
}

void configParams() {
    int size;
    string p1, p2;

    std::cout << "Розмер поля (> 3): ";
    cin >> size;
    if (size < 3) size = 3;

    std::cin.ignore();
    std::cout << "Имя игрока 1: ";
    getline(cin, p1);

    std::cout << "Имя игрока 2: ";
    getline(cin, p2);

    saveParams(size, p1, p2);
    std::cout << "Сохранено" << std::endl;
}

void showStats() {
    vector<string> stats = loadStats();
    if (stats.empty()) {
        std::cout << "История чиста." << std::endl;
    } else {
        std::cout << "История игры" << std::endl;
        for (const string& record : stats) {
            std::cout << record << std::endl;
        }
    }
}

void displayBoard(const vector<vector<char>>& board) {
    std::cout << "\nПоле игры:" << std::endl;
    for (const auto& row : board) {
        for (const char cell : row) {
            std::cout << setw(2) << cell << " ";
        }
        std::cout << std::endl;
    }
}

bool isValidMove(const vector<vector<char>>& board, int row, int col) {
    int size = board.size();
    return row >= 0 && row < size && col >= 0 && col < size && board[row][col] == EMPTY_CELL;
}

bool checkWin(const vector<vector<char>>& board, char mark) {
    int size = board.size();

    for (int i = 0; i < size; i++) {
        if (all_of(board[i].begin(), board[i].end(), [mark](char cell) { return cell == mark; }) ||
            all_of(board.begin(), board.end(), [i, mark](const vector<char>& row) { return row[i] == mark; })) {
            return true;
        }
    }
    bool diag1 = true, diag2 = true;
    for (int i = 0; i < size; i++) {
        diag1 &= (board[i][i] == mark);
        diag2 &= (board[i][size - 1 - i] == mark);
    }
    return diag1 || diag2;
}

bool isBoardFull(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        if (any_of(row.begin(), row.end(), [](char cell) { return cell == EMPTY_CELL; })) {
            return false;
        }
    }
    return true;
}

void startGame() {
    int size = loadSize();
    pair<string, string> players = loadNames();

    vector<vector<char>> board(size, vector<char>(size, EMPTY_CELL));

    string firstPlayer, secondPlayer;
    char firstMark = X_MARK, secondMark = O_MARK;

    std::cout << "Кто будет ходить первым? (1 - " << players.first << ", 2 - " << players.second << "): ";
    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        std::cout << "Некорректный выбор. Введите 1 или 2: ";
        std::cin.clear();
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (choice == 1) {
        firstPlayer = players.first;
        secondPlayer = players.second;
    } else {
        firstPlayer = players.second;
        secondPlayer = players.first;
        swap(firstMark, secondMark);
    }

    bool isFirstPlayerTurn = true;

    while (true) {
        displayBoard(board);

        string currentPlayer = isFirstPlayerTurn ? firstPlayer : secondPlayer;
        char currentMark = isFirstPlayerTurn ? firstMark : secondMark;

        int row, col;
        std::cout << currentPlayer << " (" << currentMark << "), введите координаты строки и столбца через пробел: ";
        while (!(cin >> row >> col) || !isValidMove(board, row - 1, col - 1)) {
            std::cout << "Некорректный ход. Попробуйте снова: ";
            std::cin.clear();
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        row--;
        col--;
        board[row][col] = currentMark;

        if (checkWin(board, currentMark)) {
            displayBoard(board);
            std::cout << currentPlayer << " выиграл!" << std::endl;
            saveStat(currentPlayer + " выиграл.");
            break;
        }

        if (isBoardFull(board)) {
            displayBoard(board);
            std::cout << "Ничья!" << std::endl;
            saveStat("Ничья.");
            break;
        }

        isFirstPlayerTurn = !isFirstPlayerTurn;
    }
    std::cout << "Хотите начать новую игру или выйти в меню? (1 - Новая игра, 2 - Меню): ";
    int nextAction;
    while (!(cin >> nextAction) || (nextAction != 1 && nextAction != 2)) {
        std::cout << "Некорректный выбор. Введите 1 или 2: ";
        std::cin.clear();
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (nextAction == 1) {
        startGame();
    }
}


