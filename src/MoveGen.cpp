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
  // Castling Moves
  int castlingRights = board.getCastlingRights();
  if (piece > 0 && startSquare == 60) { // White King on e1
      // King cannot castle out of check
      if (!isSquareAttacked(60, BLACK, board)) {
          // White Kingside (K -> bit 1)
          if ((castlingRights & 1) &&
              board.getPieceAt(61) == NONE &&
              board.getPieceAt(62) == NONE &&
              !isSquareAttacked(61, BLACK, board) &&
              !isSquareAttacked(62, BLACK, board)) {
              moves.push_back({60, 62, CASTLING});
          }

          // White Queenside (Q -> bit 2)
          if ((castlingRights & 2) &&
              board.getPieceAt(59) == NONE &&
              board.getPieceAt(58) == NONE &&
              board.getPieceAt(57) == NONE &&
              !isSquareAttacked(59, BLACK, board) &&
              !isSquareAttacked(58, BLACK, board)) {
              moves.push_back({60, 58, CASTLING});
          }
      }
  }
  else if (piece < 0 && startSquare == 4) { // Black King on e8
          
          // Black Kingside (k -> bit 4)
          if ((castlingRights & 4) &&
              board.getPieceAt(5) == NONE &&
              board.getPieceAt(6) == NONE &&
              !isSquareAttacked(5, WHITE, board) &&
              !isSquareAttacked(6, WHITE, board)) {
              moves.push_back({4, 6, CASTLING});
          }

          // Black Queenside (q -> bit 8)
          if ((castlingRights & 8) &&
              board.getPieceAt(3) == NONE &&
              board.getPieceAt(2) == NONE &&
              board.getPieceAt(1) == NONE &&
              !isSquareAttacked(3, WHITE, board) &&
              !isSquareAttacked(2, WHITE, board)) {
              moves.push_back({4, 2, CASTLING});
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

  for(int dCol : {-1 , 1}){
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

bool MoveGen::isSquareAttacked(int sq, int attackerColor, const Board& board) const{
  int row = sq / 8;
  int col = sq % 8;
  // Check for pawn attacks
  if(attackerColor == WHITE){
    if(row < 7){
      if(col < 7 && board.getPieceAt(sq + 9) == PAWN) return true;
      if(col > 0 && board.getPieceAt(sq + 7) == PAWN) return true;
    }
  }
  else{
    if(row > 0){
      if(col < 7 && board.getPieceAt(sq - 7) == -PAWN) return true;
      if(col > 0 && board.getPieceAt(sq - 9) == -PAWN) return true;
    }
  }

  // Check for knight attacks
  int enemyKnight = (attackerColor == WHITE) ? KNIGHT : -KNIGHT;
  for(int targetSq : knightMoves[sq]){
    if(board.getPieceAt(targetSq) == enemyKnight) return true;
  }

  // Check for king attacks
  int enemyKing   = (attackerColor == WHITE) ? KING   : -KING;
  for(int targetSq : kingMoves[sq]){
    if(board.getPieceAt(targetSq) == enemyKing) return true;
  }
  
  // Check for sliding piece attacks
  for(int dirIndex = 0; dirIndex < 8; dirIndex++){
    for(int step = 1; step <= numSquaresToEdge[sq][dirIndex]; step++){
      int targetSq = sq + directionOffsets[dirIndex] * step;
      int pieceOnTarget = board.getPieceAt(targetSq);
      if(pieceOnTarget != NONE){
        int type = std::abs(pieceOnTarget);
        if((dirIndex < 4 && (type == ROOK || type == QUEEN)) || (dirIndex >= 4 && (type == BISHOP || type == QUEEN))){
          if((attackerColor == WHITE && pieceOnTarget > 0) || (attackerColor == BLACK && pieceOnTarget < 0)){
            return true;
          }
        }
        break; // Stop searching in this direction
      }
    }
  }
  
  return false; // No attacks found
}

bool MoveGen::isKingInCheck(int kingColor, const Board& board) const{
  int targetKing = (kingColor == WHITE) ? KING : -KING;
  for(int sq = 0; sq < 64; sq++){
    if(board.getPieceAt(sq) == targetKing){
      int opponentColor = (kingColor == WHITE) ? BLACK : WHITE;
      return isSquareAttacked(sq, opponentColor, board);
    }
  }
  return false;
}

std::vector<Move> MoveGen::generateLegalMoves(Board& board) const{
  std::vector<Move> pseudoMoves = generateMoves(board);
  std::vector<Move> legalMoves;
  int movingSide = board.getSideToMove();

  for (const Move& move : pseudoMoves) {
    board.makeMove(move);
    if (!isKingInCheck(movingSide, board)) {
      legalMoves.push_back(move);
    }
    board.unmakeMove(move);
  }
  return legalMoves;
}