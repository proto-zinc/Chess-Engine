#pragma once
#include "Board.hpp"
#include "Move.hpp"
#include <vector>

class MoveGen
{
  private:
    static constexpr int directionOffsets[8] = { -8, 8, -1, 1, -9, -7, 7, 9 };
    int numSquaresToEdge[64][8];
    std::vector<int> knightMoves[64];
    std::vector<int> kingMoves[64];
    void precomputeMoveData();
    void generateSlidingMoves(int startSquare, int piece, const Board& board, std::vector<Move>& moves) const;
    void generateKnightMoves(int startSquare, int piece, const Board& board, std::vector<Move>& moves) const;
    void generateKingMoves(int startSquare, int piece, const Board& board, std::vector<Move>& moves) const;
    void generatePawnMoves(int startSquare, int piece, const Board& board, std::vector<Move>& moves) const;
    void addPawnMove(int from, int to, int promotionRow, std::vector<Move>& moves) const ;
  public:
    MoveGen(){
      precomputeMoveData();
    }
    std::vector<Move> generateMoves(const Board& board) const;
    bool isSquareAttacked(int sq, int attackerColor, const Board& board) const;
    bool isKingInCheck(int kingColor, const Board& board) const;
};

/*constexpr (Constant Expression): Tells the compiler these 8 numbers are fixed at compile time and cannot be modified while the program runs.

static (inside a class): Means the array belongs to the MoveGen class itself rather than to individual object instances—only one copy exists in memory, and it will never cause linker conflicts across multiple .cpp files. */