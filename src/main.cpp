#include <iostream> 
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <emscripten.h>
#include <cstring>

const int BOARD_SIZE = 14;
const int HERO_STORE = 6;
const int VILLAIN_STORE = 13;

enum GameMode {
    CAPTURE = 0,
    AVALANCHE = 1,
    AVALANCHE_CAPTURE = 2
};

class MancalaGame {
public:
    MancalaGame() {
        board = std::vector<int>(BOARD_SIZE, 4);
        board[HERO_STORE] = 0;
        board[VILLAIN_STORE] = 0;
        currentPlayer = 0;
        gameMode = CAPTURE;
        gameOver = false;
    }

    void setGameMode(int mode) {
        if (mode >= 0 && mode <= 2) {
            gameMode = static_cast<GameMode>(mode);
            std::cout << "Game mode set to: " << gameMode << std::endl;
        } else {
            std::cout << "Invalid game mode. Defaulting to CAPTURE mode." << std::endl;
            gameMode = CAPTURE;
        }
    }

    void makeMove(int pit) {
        if (gameOver) {
            std::cout << "Game is over. Please reset the game." << std::endl;
            return;
        }

        if (!isValidMove(pit)) {
            std::cout << "Invalid move." << std::endl;
            return;
        }

        std::cout << "Player " << currentPlayer << " selects pit " << pit << std::endl;
        std::cout << "Game mode: " << gameMode << std::endl;

        saveState();

        int stones = board[pit];
        board[pit] = 0;
        int currentIdx = pit;

        if (gameMode == AVALANCHE || gameMode == AVALANCHE_CAPTURE) {
            // Avalanche sowing
            while (true) {
                while (stones > 0) {
                    currentIdx = (currentIdx + 1) % BOARD_SIZE;
                    if (currentPlayer == 0 && currentIdx == VILLAIN_STORE) continue;
                    if (currentPlayer == 1 && currentIdx == HERO_STORE) continue;
                    board[currentIdx]++;
                    stones--;
                }
                // If last seed lands in non-empty pit on player side, keep avalanching
                if (board[currentIdx] > 1 && !isStore(currentIdx)) {
                    stones = board[currentIdx];
                    board[currentIdx] = 0;
                    std::cout << "Avalanche continues from pit " << currentIdx << std::endl;
                } else {
                    break;
                }
            }
        } else {
            // Capture sowing
            while (stones > 0) {
                currentIdx = (currentIdx + 1) % BOARD_SIZE;
                if (currentPlayer == 0 && currentIdx == VILLAIN_STORE) continue;
                if (currentPlayer == 1 && currentIdx == HERO_STORE) continue;
                board[currentIdx]++;
                stones--;
            }
        }

        // Apply capture if applicable
        applyCaptureRules(currentIdx);

        // Check if player gets to go again
        if ((currentPlayer == 0 && currentIdx == HERO_STORE) || (currentPlayer == 1 && currentIdx == VILLAIN_STORE)) {
            std::cout << "Player " << currentPlayer << " gets another turn." << std::endl;
        } else {
            switchPlayer();
        }

        // Check if game is over
        checkGameOver();

        displayBoard();
    }

    void undoMove() {
        if (history.empty()) return;
        board = history.top().first;
        currentPlayer = history.top().second;
        gameOver = false;

        history.pop();

        std::cout << "Move undone. It's player " << currentPlayer << "'s turn." << std::endl;
        displayBoard();
    }

    void resetGame() {
        board = std::vector<int>(BOARD_SIZE, 4);
        board[HERO_STORE] = 0;
        board[VILLAIN_STORE] = 0;
        currentPlayer = 0;
        gameOver = false;

        while (!history.empty()) {
            history.pop();
        }

        std::cout << "Game reset!" << std::endl;
        displayBoard();
    }

    void displayBoard() const {
        std::cout << "Current Board State:" << std::endl;
        std::cout << "Villain's Store (" << VILLAIN_STORE << "): " << board[VILLAIN_STORE] << std::endl;
        std::cout << "Villain's Pits: ";
        for (int i = 12; i >= 7; i--) {
            std::cout << "[" << i << "]: " << board[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Hero's Pits: ";
        for (int i = 0; i <= 5; i++) {
            std::cout << "[" << i << "]: " << board[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Hero's Store (" << HERO_STORE << "): " << board[HERO_STORE] << std::endl;
    }

    int getCurrentPlayer() {
        return currentPlayer;
    }

    char* getBoard() {
        static char result[512];
        std::ostringstream boardJson;
        boardJson << "{";

        boardJson << "\"rowvillain\": [";
        for (int i = 0; i < 6; i++) {
            boardJson << board[12 - i];
            if (i < 5) boardJson << ", ";
        }
        boardJson << "],";

        boardJson << "\"rowhero\": [";
        for (int i = 0; i < 6; i++) {
            boardJson << board[i];
            if (i < 5) boardJson << ", ";
        }
        boardJson << "],";

        boardJson << "\"storevillain\": " << board[VILLAIN_STORE] << ",";
        boardJson << "\"storehero\": " << board[HERO_STORE];
        boardJson << "}";

        std::string boardState = boardJson.str();
        strncpy(result, boardState.c_str(), sizeof(result) - 1);
        result[sizeof(result) - 1] = '\0';
        return result;
    }

private:
    std::vector<int> board;
    std::stack<std::pair<std::vector<int>, int>> history;
    int currentPlayer;
    GameMode gameMode;
    bool gameOver;

    void saveState() {
        history.push({board, currentPlayer});
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == 0) ? 1 : 0;
    }

    bool isValidMove(int pit) const {
        if (pit < 0 || pit >= BOARD_SIZE || isStore(pit)) return false;
        if (board[pit] == 0) return false;

        if (currentPlayer == 0 && (pit < 0 || pit > 5)) return false;
        if (currentPlayer == 1 && (pit < 7 || pit > 12)) return false;

        return true;
    }

    bool isStore(int pit) const {
        return pit == HERO_STORE || pit == VILLAIN_STORE;
    }

    bool pitBelongsToCurrentPlayer(int pit) const {
        if (currentPlayer == 0 && pit >= 0 && pit <= 5) return true;
        if (currentPlayer == 1 && pit >= 7 && pit <= 12) return true;
        return false;
    }

    void applyCaptureRules(int lastIndex) {
        if (isStore(lastIndex)) return;

        if (gameMode == CAPTURE || gameMode == AVALANCHE_CAPTURE) {
            if (board[lastIndex] == 1 && pitBelongsToCurrentPlayer(lastIndex)) {
                int oppositeIndex = 12 - lastIndex;
                if (board[oppositeIndex] > 0) {
                    int captured = board[oppositeIndex] + board[lastIndex];
                    board[oppositeIndex] = 0;
                    board[lastIndex] = 0;

                    if (currentPlayer == 0) {
                        board[HERO_STORE] += captured;
                        std::cout << "Player " << currentPlayer << " captures " << captured << " stones." << std::endl;
                    } else {
                        board[VILLAIN_STORE] += captured;
                        std::cout << "Player " << currentPlayer << " captures " << captured << " stones." << std::endl;
                    }
                }
            }
        }
    }

    void checkGameOver() {
        int heroPitsTotal = 0;
        int villainPitsTotal = 0;

        for (int i = 0; i <= 5; i++) {
            heroPitsTotal += board[i];
        }

        for (int i = 7; i <= 12; i++) {
            villainPitsTotal += board[i];
        }

        if (heroPitsTotal == 0 || villainPitsTotal == 0) {
            gameOver = true;
            // Move remaining stones to respective stores
            board[HERO_STORE] += heroPitsTotal;
            board[VILLAIN_STORE] += villainPitsTotal;

            for (int i = 0; i <= 5; i++) {
                board[i] = 0;
            }

            for (int i = 7; i <= 12; i++) {
                board[i] = 0;
            }

            // Announce winner
            std::cout << "Game Over!" << std::endl;
            if (board[HERO_STORE] > board[VILLAIN_STORE]) {
                std::cout << "Hero wins!" << std::endl;
            } else if (board[HERO_STORE] < board[VILLAIN_STORE]) {
                std::cout << "Villain wins!" << std::endl;
            } else {
                std::cout << "It's a tie!" << std::endl;
            }
        }
    }
};

MancalaGame game;

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void makeMove(int pit) {
        game.makeMove(pit);
    }

    EMSCRIPTEN_KEEPALIVE
    void undoMove() {
        game.undoMove();
    }

    EMSCRIPTEN_KEEPALIVE
    void displayBoard() {
        game.displayBoard();
    }

    EMSCRIPTEN_KEEPALIVE
    void resetGame() {
        game.resetGame();
    }

    EMSCRIPTEN_KEEPALIVE
    int getCurrentPlayer() {
        return game.getCurrentPlayer();
    }

    EMSCRIPTEN_KEEPALIVE
    char* getBoard() {
        return game.getBoard();
    }

    EMSCRIPTEN_KEEPALIVE
    void setGameMode(int mode) {
        game.setGameMode(mode);
    }

}

int main() {
    game.displayBoard();
    return 0;
}
