#pragma once
#include "Types.hpp"
#include "Move.hpp"
#include <array>
#include <string>
#include <vector>

struct UndoState {
    int capturedPiece ;
    int castlingRights ;
    int enPassantSquare ;
};

class Board {
private:
    int board[64];
    int sideToMove;
    int castlingRights;
    int enPassantSquare; // none -> -1
    std:: vector<UndoState> history ;
    static constexpr int castlingRightsMask[64] = {
         7, 15, 15, 15,  3, 15, 15, 11, // Rank 8: a8(7), e8(3), h8(11)
        15, 15, 15, 15, 15, 15, 15, 15, // Rank 7
        15, 15, 15, 15, 15, 15, 15, 15, // Rank 6
        15, 15, 15, 15, 15, 15, 15, 15, // Rank 5
        15, 15, 15, 15, 15, 15, 15, 15, // Rank 4
        15, 15, 15, 15, 15, 15, 15, 15, // Rank 3
        15, 15, 15, 15, 15, 15, 15, 15, // Rank 2
        13, 15, 15, 15, 12, 15, 15, 14  // Rank 1: a1(13), e1(12), h1(14)
    };
public:
    void setupStartingPosition();
    void printBoard();
    int squareIndex(int rank, int file);
    void loadPositionFromFEN(const std::string& fen);
    void makeMove(Move move) ;
    void unmakeMove(Move move) ;
    int getPieceAt(int sq) const;
    int getSideToMove() const;
    int getEnPassantSquare() const;
    int getCastlingRights() const;
};
