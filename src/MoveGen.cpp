#include "Move.hpp"
#include "MoveGen.hpp"
#include <cmath>

void MoveGen::precomputeMoveData(){
  for (int sq = 0; sq < 64; sq++) {
    int row = sq / 8 ;
    int col = sq % 8 ;
    int north = row ;
    int south = 7 - row ;
    int west = col ;
    int east = 7 - col ;
    int north_west = std::min(north , west) ;
    int north_east = std::min(north , east) ;
    int south_west = std::min(south , west) ;
    int south_east = std::min(south , east) ;
    numSquaresToEdge[sq][0] = north ;
    numSquaresToEdge[sq][1] = south ;
    numSquaresToEdge[sq][2] = west ;
    numSquaresToEdge[sq][3] = east ;
    numSquaresToEdge[sq][4] = north_west ;
    numSquaresToEdge[sq][5] = north_east ;
    numSquaresToEdge[sq][6] = south_west ;
    numSquaresToEdge[sq][7] = south_east ;
  }
}

std::vector<Move> MoveGen::generateMoves(const Board& board) const{
  std::vector<Move> moves;
  int sideToMove = board.getSideToMove();
  for (int startSquare = 0 ; startSquare < 64 ; startSquare++) {
    int piece = board.getPieceAt(startSquare);
    if(piece == NONE || (sideToMove == WHITE && piece < 0) || (sideToMove == BLACK && piece > 0)) continue;
    int pieceType = std::abs(piece);
    if(pieceType == ROOK || pieceType == QUEEN || pieceType == BISHOP){
      generateSlidingMoves(startSquare, piece, board, moves);
    }
  }
  return moves ;
}

void MoveGen::generateSlidingMoves(int startSquare, int piece, const Board& board, std::vector<Move>& moves) const{

  int pieceType = std::abs(piece);
  int startDir = (pieceType == BISHOP) ? 4 : 0;
  int endDir   = (pieceType == ROOK)   ? 3 : 7;
  for (int dirIndex = startDir; dirIndex <= endDir; dirIndex++) {

    for(int step = 1 ; step <= numSquaresToEdge[startSquare][dirIndex] ; step++){

      int targetSquare = startSquare + directionOffsets[dirIndex] * step;
      int pieceOnTarget = board.getPieceAt(targetSquare);

      // Friendly Piece
      if(piece * pieceOnTarget > 0) break ;

      // Enemy Piece
      else if(piece * pieceOnTarget < 0){
        moves.push_back({startSquare, targetSquare});
        break ;
      }
      // Target Piece is empty. Insert piece and keep on sliding.
      else{
        moves.push_back({startSquare, targetSquare});
      }

    }
  }
}
