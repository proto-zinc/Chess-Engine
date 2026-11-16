#include "Board.hpp"
#include "Move.hpp"
#include "MoveGen.hpp"
#include <iostream>
#include <vector>

int main(){
  Board chessBoard;
  const std::string startingFEN = "4kb1r/p2rqppp/5n2/1B2p1B1/4P3/1Q6/PPP2PPP/2KR4 b k - 0 14";
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