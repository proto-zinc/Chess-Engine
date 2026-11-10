#pragma once
#include "Types.hpp"
#include <array>
#include <string>

class Board {
private:
    int board[64];
public:
    void setupStartingPosition(const std::string& fen);
    void printBoard();
    int squareIndex(int rank, int file);
    void loadPositionFromFEN(const std::string& fen);
};