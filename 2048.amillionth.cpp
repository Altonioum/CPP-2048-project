#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

const int SIZE = 5;
const string SAVE_FILE = "save2048.txt";

class Game2048 {
private:
    int board[SIZE][SIZE];
    int score;

    void spawnTile() {
        vector<pair<int, int>> emptyCells;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    emptyCells.push_back({i, j});
                }
            }
        }

        if (!emptyCells.empty()) {
            int randIndex = rand() % emptyCells.size();
            int r = emptyCells[randIndex].first;
            int c = emptyCells[randIndex].second;
            // 90% chance for 2, 10% chance for 4
            board[r][c] = (rand() % 10 == 0) ? 4 : 2;
        }
    }

    void rotateBoard() {
        int temp[SIZE][SIZE];
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                temp[j][SIZE - 1 - i] = board[i][j];
            }
        }
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = temp[i][j];
            }
        }
    }

    bool slideLeftRow(int row[]) {
        bool moved = false;
        int temp[SIZE] = {0};
        int pos = 0;

        // Slide non-zero elements together
        for (int i = 0; i < SIZE; i++) {
            if (row[i] != 0) {
                temp[pos++] = row[i];
            }
        }

        // Merge adjacent identical elements
        for (int i = 0; i < SIZE - 1; i++) {
            if (temp[i] != 0 && temp[i] == temp[i + 1]) {
                temp[i] *= 2;
                score += temp[i];
                temp[i + 1] = 0;
                moved = true;
            }
        }

        // Slide again after merging
        int finalRow[SIZE] = {0};
        pos = 0;
        for (int i = 0; i < SIZE; i++) {
            if (temp[i] != 0) {
                finalRow[pos++] = temp[i];
            }
        }

        // Check if row changed
        for (int i = 0; i < SIZE; i++) {
            if (row[i] != finalRow[i]) {
                moved = true;
            }
            row[i] = finalRow[i];
        }

        return moved;
    }

    bool moveLeft() {
        bool moved = false;
        for (int i = 0; i < SIZE; i++) {
            if (slideLeftRow(board[i])) {
                moved = true;
            }
        }
        return moved;
    }

public:
    Game2048() {
        initNewGame();
    }

    void initNewGame() {
        score = 0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = 0;
            }
        }
        spawnTile();
        spawnTile();
    }

    void drawBoard() {
        cout << "\n=====================================\n";
        cout << "            2048 (5x5)              \n";
        cout << "            Score: " << score << "\n";
        cout << "=====================================\n\n";

        for (int i = 0; i < SIZE; i++) {
            cout << "+------+------+------+------+------+\n|";
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    cout << "      |";
                } else {
                    cout << setw(5) << board[i][j] << " |";
                }
            }
            cout << "\n";
        }
        cout << "+------+------+------+------+------+\n\n";
    }

    void move(char dir) {
        bool moved = false;
        dir = toupper(dir);

        if (dir == 'A') { // Left
            moved = moveLeft();
        } else if (dir == 'D') { // Right
            rotateBoard();
            rotateBoard();
            moved = moveLeft();
            rotateBoard();
            rotateBoard();
        } else if (dir == 'W') { // Up
            rotateBoard();
            rotateBoard();
            rotateBoard();
            moved = moveLeft();
            rotateBoard();
        } else if (dir == 'S') { // Down
            rotateBoard();
            moved = moveLeft();
            rotateBoard();
            rotateBoard();
            rotateBoard();
        }

        if (moved) {
            spawnTile();
        } else {
            cout << ">> Invalid move or move not possible!\n";
        }
    }

    void saveGame() {
        ofstream outFile(SAVE_FILE);
        if (!outFile) {
            cout << ">> Error saving game!\n";
            return;
        }
        outFile << score << "\n";
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                outFile << board[i][j] << " ";
            }
            outFile << "\n";
        }
        outFile.close();
        cout << ">> Game saved successfully to '" << SAVE_FILE << "'!\n";
    }

    void loadGame() {
        ifstream inFile(SAVE_FILE);
        if (!inFile) {
            cout << ">> Save file not found!\n";
            return;
        }
        inFile >> score;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                inFile >> board[i][j];
            }
        }
        inFile.close();
        cout << ">> Game loaded successfully!\n";
    }
};

int main() {
    srand(time(0));
    Game2048 game;
    char choice;

    while (true) {
        game.drawBoard();

        cout << "Controls:\n";
        cout << " [W] Up   [S] Down   [A] Left   [D] Right\n";
        cout << " [N] New Game   [V] Save Game   [L] Load Game   [Q] Quit\n";
        cout << "-------------------------------------\n";
        cout << "Enter command: ";
        cin >> choice;

        choice = toupper(choice);

        if (choice == 'Q') {
            cout << "Thanks for playing!\n";
            break;
        } else if (choice == 'N') {
            game.initNewGame();
            cout << ">> New game started!\n";
        } else if (choice == 'V') {
            game.saveGame();
        } else if (choice == 'L') {
            game.loadGame();
        } else if (choice == 'W' || choice == 'A' || choice == 'S' || choice == 'D') {
            game.move(choice);
        } else {
            cout << ">> Invalid command! Please try again.\n";
        }
    }

    return 0;
}