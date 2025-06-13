#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

const string SETTINGS_FILE = "settings.txt";
const string HISTORY_FILE = "history.txt";

struct Settings {
    int fieldSize = 3;
    string player1 = "Player 1";
    string player2 = "Player 2";
};

Settings loadSettings() {
    Settings settings;
    ifstream file(SETTINGS_FILE);
    if (file.is_open()) {
        file >> settings.fieldSize;
        file.ignore();
        getline(file, settings.player1);
        getline(file, settings.player2);
    } else {
        cout << "Error." << endl;
        cout << "Unable to open settings file. Using default settings." << endl;
    }
    file.close();
    return settings;
}

void saveSettings(const Settings& settings) {
    ofstream file(SETTINGS_FILE);
    if (file.is_open()) {
        file << settings.fieldSize << endl;
        file << settings.player1 << endl;
        file << settings.player2 << endl;
    } else {
        cout << "Unable to open settings file for saving." << endl;
    }
    file.close();
}

void showHistory() {
    ifstream file(HISTORY_FILE);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
    } else {
        cout << "Game history is empty." << endl;
    }
    file.close();
}

void saveGameResult(const string& player1, const string& player2, int fieldSize, const string& winner) {
    ofstream file(HISTORY_FILE, ios::app);
    if (file.is_open()) {
        file << player1 << " vs " << player2 << " | Field: " << fieldSize << "x" << fieldSize << " | Winner: " << winner << endl;
    } else {
        cout << "Unable to open history file for saving game result." << endl;
    }
    file.close();
}

void displayBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

bool isWin(const vector<vector<char>>& board, char symbol) {
    for (int i = 0; i < board.size(); i++) {
        bool rowWin = true, colWin = true;
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] != symbol) rowWin = false;
            if (board[j][i] != symbol) colWin = false;
        }
        if (rowWin || colWin) return true;
    }

    bool diagWin1 = true, diagWin2 = true;
    for (int i = 0; i < board.size(); i++) {
        if (board[i][i] != symbol) diagWin1 = false;
        if (board[i][board.size() - i - 1] != symbol) diagWin2 = false;
    }
    return diagWin1 || diagWin2;
}

bool isBoardFull(const vector<vector<char>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] == '.') {
                return false;
            }
        }
    }
    return true;
}

void settingsMenu(Settings& settings) {
    cout << "=== Settings ===" << endl;
    cout << "1. Set field size (minimum 3x3)" << endl;
    cout << "2. Set Player 1 name" << endl;
    cout << "3. Set Player 2 name" << endl;
    cout << "0. Return to main menu" << endl;

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "Enter field size (minimum 3): ";
            cin >> settings.fieldSize;
            if (settings.fieldSize < 3)
                settings.fieldSize = 3;
            break;
        case 2:
            cout << "Enter Player 1 name: ";
            cin.ignore();
            getline(cin, settings.player1);
            break;
        case 3:
            cout << "Enter Player 2 name: ";
            cin.ignore();
            getline(cin, settings.player2);
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice!" << endl;
    }

    saveSettings(settings);
    cout << "Settings saved!" << endl;
}

void afterGameMenu(Settings& settings) {
    while (true) {
        cout << "What would you like to do next?" << endl;
        cout << "1. Start a new game" << endl;
        cout << "2. Return to main menu" << endl;
        cout << "Your choice: ";
        
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            return;  
        case 2:
            return;  
        default:
            cout << "Invalid choice!" << endl;
        }
    }
}

void mainMenu() {
    Settings settings = loadSettings();
    while (true) {
        cout << "=== Tic-Tac-Toe ===" << endl;
        cout << "1. Start New Game" << endl;
        cout << "2. Settings" << endl;
        cout << "3. Game History" << endl;
        cout << "4. Exit" << endl;
        cout << "Your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Who will play first?" << endl;
            cout << "1. " << settings.player1 << endl;
            cout << "2. " << settings.player2 << endl;
            int firstPlayerChoice;
            cin >> firstPlayerChoice;

            string currentPlayer = (firstPlayerChoice == 1) ? settings.player1 : settings.player2;
            char currentSymbol = 'X';
            string opponent = (firstPlayerChoice == 1) ? settings.player2 : settings.player1;
            char opponentSymbol = 'O';

            vector<vector<char>> board(settings.fieldSize, vector<char>(settings.fieldSize, '.'));

            while (true) {
                displayBoard(board);
                int row, col;
                cout << currentPlayer << "'s turn (" << currentSymbol << "). Enter row and column (0-based): ";
                cin >> row >> col;

                if (row >= 0 && row < settings.fieldSize && col >= 0 && col < settings.fieldSize && board[row][col] == '.') {
                    board[row][col] = currentSymbol;

                    if (isWin(board, currentSymbol)) {
                        displayBoard(board);
                        cout << currentPlayer << " wins!" << endl;
                        saveGameResult(settings.player1, settings.player2, settings.fieldSize, currentPlayer);
                        afterGameMenu(settings);
                        break;
                    }

                    if (isBoardFull(board)) {
                        displayBoard(board);
                        cout << "It's a draw!" << endl;
                        saveGameResult(settings.player1, settings.player2, settings.fieldSize, "Draw");
                        afterGameMenu(settings); 
                        break;
                    }

                    swap(currentPlayer, opponent);
                    swap(currentSymbol, opponentSymbol);
                } else {
                    cout << "Invalid move, try again." << endl;
                }
            }
            break;
        }
        case 2:
            settingsMenu(settings);
            break;
        case 3:
            showHistory();
            break;
        case 4:
            cout << "Exiting program." << endl;
            return;
        default:
            cout << "Invalid choice!" << endl;
        }

        cout << endl;
    }
}

int main() {
    mainMenu();
    return 0;
}
