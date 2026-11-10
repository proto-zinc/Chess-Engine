#include "Board.hpp"
#include <iostream>

void Board::setupStartingPosition(){

  for (int i = 0; i < 64; i++) {
    if(i >= A2 && i <= H2){
      board[i] = PAWN ;
    }
    else if(i >= A7 && i <= H7){
      board[i] = -PAWN ;
    }
    else if(i == A1 || i == H1){
      board[i] = ROOK ;
    }
    else if(i == B1 || i == G1){
      board[i] = KNIGHT;
    }
    else if(i == C1 || i == F1){
      board[i] = BISHOP ;
    }
    else if(i == D1){
      board[i] = QUEEN ;
    }
    else if(i == E1){
      board[i] = KING ;
    }
    else if(i == A8 || i == H8){
      board[i] = -(ROOK) ;
    }
    else if(i == B8 || i == G8){
      board[i] = -(KNIGHT) ;
    }
    else if(i == C8 || i == F8){
      board[i] = -(BISHOP) ;
    }
    else if(i == D8){
      board[i] = -(QUEEN) ;
    }
    else if(i == E8){
      board[i] = -(KING) ;
    }
    else{
      board[i] = NONE;
    }
  }
}

void Board::printBoard(){
  
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << rank + 1 << "  ";
    for (int file = 0; file < 8; file++) {
      int index = squareIndex(file, rank);
      int piece = board[index];
      char pieceChar;

      switch (piece) {
        case PAWN: pieceChar = 'P'; break;
        case KNIGHT: pieceChar = 'N'; break;
        case BISHOP: pieceChar = 'B'; break;
        case ROOK: pieceChar = 'R'; break;
        case QUEEN: pieceChar = 'Q'; break;
        case KING: pieceChar = 'K'; break;
        case -PAWN: pieceChar = 'p'; break;
        case -KNIGHT: pieceChar = 'n'; break;
        case -BISHOP: pieceChar = 'b'; break;
        case -ROOK: pieceChar = 'r'; break;
        case -QUEEN: pieceChar = 'q'; break;
        case -KING: pieceChar = 'k'; break;
        default: pieceChar = '.'; 
      }
      std::cout << pieceChar << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n   a b c d e f g h\n";
}

int Board::squareIndex(int file, int rank){
  return (rank * 8) + file;
}