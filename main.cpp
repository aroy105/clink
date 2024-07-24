#include <iostream> 
#include <vector>
#include <stack>

// Mancala board has 14 holes. Each player has 6 holes and 1 store (scoring area).
const int BOARD_SIZE = 14;

// We will represent the indices of the stores below.
const int HERO_STORE = 6;
const int VILLAIN_STORE = 13;

class MancalaGame {
public:
    MancalaGame() {
        board = std::vector<int>(BOARD_SIZE, 4);
        board[HERO_STORE] = 0;
        board[VILLAIN_STORE] = 0;
        currentPlayer = 0;
    }

    void makeMove(int pit) {
        if (!isValidMove(pit)) return;
        saveState();

        // Pick up the stones from a particular pit
        int stones = board[pit];
        board[pit] = 0;
        int currentIdx = pit;

        // Distribute the stones
        while (stones > 0) {
            currentIdx = (currentIdx + 1) % BOARD_SIZE;
            if (currentIdx == VILLAIN_STORE && currentPlayer == 0) continue;
            if (currentIdx == HERO_STORE && currentPlayer == 1) continue;
            board[currentIdx]++;
            stones--;
        }

        applyRules(currentIdx);
        switchPlayer();
    }

    void undoMove() {
        if (history.empty()) return;

        // Grab the previous state
        board = history.top().first;
        currentPlayer = history.top().second;

        history.pop();
    }

    void displayBoard() const {
        for (int i = 0; i < BOARD_SIZE; i++) {
            std::cout << board[i] << " ";
        }
        std::cout << std::endl;
    }

    int getCurrentPlayer() const {
        return currentPlayer;
    }

private:
    // Store the board as an array
    std::vector<int> board;
    // Each game state stores the board and player at that time
    std::stack<std::pair<std::vector<int>, int>> history;
    int currentPlayer;

    void saveState() {
        history.push({board, currentPlayer});
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == 0) ? 1 : 0;
    }

    bool isValidMove(int pit) const {
        if (pit < 0 || pit >= BOARD_SIZE || pit == HERO_STORE || pit == VILLAIN_STORE) return false;
        if ((currentPlayer == 0 && pit >= HERO_STORE) || (currentPlayer == 1 && pit < HERO_STORE)) return false;
    }
    void applyRules(int lastIndex) {
        if (currentPlayer == 0 && lastIndex == HERO_STORE) {
            switchPlayer();
        }
        if (currentPlayer == 1 && lastIndex == VILLAIN_STORE) {
            switchPlayer();
        }

    }
    
};

int main() {
    MancalaGame game;

    game.displayBoard();

    return 0;
}