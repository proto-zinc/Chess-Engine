#include "Move.hpp"
#include "MoveGen.hpp"
#include <cmath>
#include <algorithm>

void MoveGen::precomputeMoveData(){

  // For non - sliding pieces
  const int knightRowDeltas[8] = { -2, -2, -1, -1,  1,  1,  2,  2 };
  const int knightColDeltas[8] = { -1,  1, -2,  2, -2,  2, -1,  1 };

  const int kingRowDeltas[8] = { -1, -1, -1,  0,  0,  1,  1,  1 };
  const int kingColDeltas[8] = { -1,  0,  1, -1,  1, -1,  0,  1 };

  for (int sq = 0; sq < 64; sq++) {
    int row = sq / 8 ;
    int col = sq % 8 ;
    // For sliding pieces
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

    // For non - sliding pieces
      for (int i = 0; i < 8; i++) {
        // Knight 
        int targetRow = row + knightRowDeltas[i];
        int targetCol = col + knightColDeltas[i];
        if((targetRow >= 0 && targetRow < 8) && (targetCol >= 0 && targetCol < 8)){
          int targetsq = (targetRow * 8) + targetCol ;
          knightMoves[sq].emplace_back(targetsq) ;
        }

        // King
        targetRow = row + kingRowDeltas[i];
        targetCol = col + kingColDeltas[i];
        if((targetRow >= 0 && targetRow < 8) && (targetCol >= 0 && targetCol < 8)){
          int targetsq = (targetRow * 8) + targetCol ;
          kingMoves[sq].emplace_back(targetsq) ;
        }
      }
  }
}

std::vector<Move> MoveGen::generateMoves(const Board& board) const{
  std::vector<Move> moves;
  int sideToMove = board.getSideToMove();
  for (int startSquare = 0 ; startSquare < 64 ; startSquare++) {
    int piece = board.getPieceAt(startSquare);
    if(piece == NONE || (sideToMove == WHITE && piece < 0) || (sideToMove == BLACK && piece > 0)) continue;
    int pieceType = std::abs(piece);
    // Sliding Pieces
    if(pieceType == ROOK || pieceType == QUEEN || pieceType == BISHOP){
      generateSlidingMoves(startSquare, piece, board, moves);
    }
    // Non - Sliding Pieces
    else if(pieceType == KING){
      generateKingMoves(startSquare, piece, board , moves) ;
    }

    else if(pieceType == KNIGHT){
      generateKnightMoves(startSquare, piece, board , moves) ;
    }
    else if(pieceType == PAWN){
      generatePawnMoves(startSquare, piece, board , moves) ;
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

void MoveGen::generateKnightMoves(int startSquare, int piece, const Board& board, std::vector<Move>& moves) const{
  for (int targetSq : knightMoves[startSquare]) {
      int pieceOnTarget = board.getPieceAt(targetSq);
      // Enemy Piece or, Target Piece is empty. Insert piece but don't break because knight can still move
      if(piece * pieceOnTarget <= 0){
        moves.push_back({startSquare, targetSq});
      }
  } 
}

void MoveGen::generateKingMoves(int startSquare, int piece, const Board& board, std::vector<Move>& moves) const{
  for (int targetSq : kingMoves[startSquare]) {
      int pieceOnTarget = board.getPieceAt(targetSq);
      // Enemy Piece or, Target Piece is empty. Insert piece but don't break because king can still move
      if(piece * pieceOnTarget <= 0){
        moves.push_back({startSquare, targetSq});
      }
  } 
}

void MoveGen::addPawnMove(int from, int to, int promotionRow, std::vector<Move>& moves) const{
  if (to / 8 == promotionRow) {
      moves.push_back({from, to, PROMOTE_TO_QUEEN});
      moves.push_back({from, to, PROMOTE_TO_ROOK});
      moves.push_back({from, to, PROMOTE_TO_BISHOP});
      moves.push_back({from, to, PROMOTE_TO_KNIGHT});
    }
    else {
      moves.push_back({from, to, NO_FLAG});
    }
}

void MoveGen::generatePawnMoves(int startSquare, int piece, const Board& board, std::vector<Move>& moves) const{
  
  int row = startSquare / 8 ;
  int col = startSquare % 8 ;
  int forwardOffset = (piece > 0) ? -8 : 8 ;
  int startRow      = (piece > 0) ? 6 : 1 ;
  int promotionRow  = (piece > 0) ? 0 : 7 ;
  int oneStep = startSquare + forwardOffset;

  if(board.getPieceAt(oneStep) == NONE){
    addPawnMove(startSquare , oneStep , promotionRow , moves) ;
    if(row == startRow){
      int twoSteps = startSquare + forwardOffset * 2;
      if(board.getPieceAt(twoSteps) == NONE){
        moves.push_back({startSquare, twoSteps, NO_FLAG}) ;
      }
    }
  }

  for(int dCol = -1 ; dCol <= 1 ; dCol++){
    int targetCol = col + dCol;
    if(targetCol >= 0 && targetCol < 8){
      int targetSq = startSquare + forwardOffset + dCol;
      int pieceOnTarget = board.getPieceAt(targetSq);
      if(piece * pieceOnTarget < 0){
        addPawnMove(startSquare , targetSq , promotionRow , moves) ;
      }
      else if(targetSq == board.getEnPassantSquare()){
        moves.push_back({startSquare , targetSq , EN_PASSANT}) ;
      }
    }
  }
}
