#include "Board.hpp"
#include "Move.hpp"
#include "MoveGen.hpp"
#include <iostream>
#include <vector>

int main(){
  Board chessBoard;
  const std::string startingFEN = "8/8/4k3/8/2pP4/8/B7/4K3 b - d3 0 1";
  chessBoard.loadPositionFromFEN(startingFEN);
  chessBoard.printBoard();

  MoveGen moveGen ;
  std::vector<Move> moves = moveGen.generateMoves(chessBoard) ;
  std::cout << "\nGenerated " << moves.size() << " pseudo-legal sliding moves:\n";
  for (const Move& m : moves) {
      std::cout << moveToString(m) << " ";
  }
  std::cout << "\n";
  std::vector<Move> legalMoves = moveGen.generateLegalMoves(chessBoard) ;
  std::cout << "\nGenerated " << legalMoves.size() << " legal sliding moves:\n";
  for (const Move& m : legalMoves) {
      std::cout << moveToString(m) << " ";
  }
  std::cout << "\n";

  return 0 ;
}