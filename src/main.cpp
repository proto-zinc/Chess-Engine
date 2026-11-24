#include "Board.hpp"
#include "Move.hpp"
#include "MoveGen.hpp"
#include <iostream>
#include <vector>

int main(){
  Board chessBoard;
  const std::string startingFEN = "r3k2r/8/8/8/5b2/8/8/R3K2R w KQkq - 0 1";
  chessBoard.loadPositionFromFEN(startingFEN);
  chessBoard.printBoard();

  MoveGen moveGen ;
  std::vector<Move> moves = moveGen.generateMoves(chessBoard) ;
  std::cout << "\nGenerated " << moves.size() << " pseudo-legal sliding moves:\n";
  for (const Move& m : moves) {
      std::cout << moveToString(m) << " ";
  }
  std::cout << "\n";
  std::cout << moveGen.isKingInCheck(BLACK, chessBoard) << "\n" ;
  std::cout << moveGen.isKingInCheck(WHITE, chessBoard) << "\n" ;
  std::cout << moveGen.isSquareAttacked(45, WHITE, chessBoard) << "\n" ; // D7

  return 0 ;
}