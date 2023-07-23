#include "queen_attack.h"
#include <vector>
#include <iostream>
const int ROWS = 8;
const int COLS = 8;

namespace queen_attack{
    const auto make_pair(int x, int y){

    }
    class ChessBoard {
    private:
        const int SIZE = 8;
        std::vector<std::vector<char>> board;

    public:
        ChessBoard() {
            board.resize(SIZE, std::vector<char>(SIZE,' '));
        }
        void printBoard() const{
            for (int i=0;i<SIZE; i++){
                for (int j=0; j<SIZE; j++){
                    std::cout << board[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
    };
    bool can_attack(int arr[ROWS][COLS]){

    }
}

int main(){
    int chessBoard[ROWS][COLS];

    return 0;
}
