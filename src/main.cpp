#include "Board.hpp"

int main(){
  Board chessBoard;
  const std::string startingFEN = "4kb1r/p2rqppp/5n2/1B2p1B1/4P3/1Q6/PPP2PPP/2KR4 b k - 0 14";
  chessBoard.setupStartingPosition(startingFEN);
  chessBoard.printBoard();

  return 0 ;
}