#include "Board.hpp"
#include "Move.hpp"
#include "MoveGen.hpp"
#include <iostream>
#include <vector>

int main(){
  Board chessBoard;
  const std::string startingFEN = "8/P7/8/8/8/8/8/k6K w - - 0 1";
  chessBoard.loadPositionFromFEN(startingFEN);
  chessBoard.printBoard();

  MoveGen moveGen ;
  std::vector<Move> moves = moveGen.generateMoves(chessBoard) ;
  std::cout << "\nGenerated " << moves.size() << " pseudo-legal sliding moves:\n";
  for (const Move& m : moves) {
      std::cout << moveToString(m) << " ";
  }
  std::cout << "\n";

  return 0 ;
}