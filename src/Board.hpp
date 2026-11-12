#pragma once
#include "Types.hpp"
#include <array>
#include <string>

class Board {
private:
    int board[64];
    int sideToMove;
    int castlingRights;
    int enPassantSquare; // none -> -1
public:
    void setupStartingPosition();
    void printBoard();
    int squareIndex(int rank, int file);
    void loadPositionFromFEN(const std::string& fen);
};