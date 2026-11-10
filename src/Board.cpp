#include "Board.hpp"
#include <iostream>

void Board::setupStartingPosition(const std::string& fen){
  loadPositionFromFEN(fen);
}

void Board::printBoard(){
  
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << rank + 1 << "  ";
    for (int file = 0; file < 8; file++) {
      int index = squareIndex(rank, file);
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
        default: pieceChar = '-'; 
      }
      std::cout << pieceChar << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n   a b c d e f g h\n";
}

int Board::squareIndex(int rank, int file){
  return ((7 - rank) * 8) + file;
}

void Board::loadPositionFromFEN(const std::string& fen){
  // Clear the internal board
  for (int i = 0; i < 64; i++) {
    board[i] = 0 ;
  }
  int square = 0 ;
  for(char c: fen){

    if(c == ' ') break ;

    else if(c == '/') continue ;

    else if(c >= '1' && c <= '8'){
      square += (c - '0') ;
    }

    else{
      if(c == 'p') board[square] = - PAWN ;
      else if(c == 'P') board[square] = PAWN ;
      else if(c == 'n') board[square] = - KNIGHT ;
      else if(c == 'N') board[square] = KNIGHT ;
      else if(c == 'b') board[square] = - BISHOP ;
      else if(c == 'B') board[square] = BISHOP ;
      else if(c == 'r') board[square] = - ROOK ;
      else if(c == 'R') board[square] = ROOK ;
      else if(c == 'q') board[square] = - QUEEN ;
      else if(c == 'Q') board[square] = QUEEN ;
      else if(c == 'k') board[square] = - KING ;
      else if(c == 'K') board[square] = KING ;
      square++ ;
    }
  }
}