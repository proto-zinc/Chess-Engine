#pragma once
#include "Types.hpp"
#include <array>

class Board {
private:
    int board[64];
public:
    void setupStartingPosition();
    void printBoard();
    int squareIndex(int file, int rank);
};