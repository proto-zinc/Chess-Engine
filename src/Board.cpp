#include "Board.hpp"
#include <iostream>
#include <sstream>

void Board::setupStartingPosition(){
  history.clear();
  loadPositionFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
  std::cout << "   a b c d e f g h\n\n";
  std::cout << "Side to move: " << (sideToMove == WHITE ? "White" : "Black") << "\n";
  std::cout << "Castling rights: " << castlingRights << "\n";
  std::cout << "En passant square: " << enPassantSquare << "\n";
}

int Board::squareIndex(int rank, int file){
  return ((7 - rank) * 8) + file;
}

void Board::loadPositionFromFEN(const std::string& fen){
  history.clear();
  // Clear the internal board
  for (int i = 0; i < 64; i++) {
    board[i] = 0 ;
  }
  // Loading position from FEN
  std::stringstream ss(fen);
  std::string piecePlacement, activeColor, castling, enPassant;
  int halfmoveClock = 0;
  int fullmoveNumber = 1;
  ss >> piecePlacement >> activeColor >> castling >> enPassant >> halfmoveClock >> fullmoveNumber;

  int square = 0 ;
  // Placing the pieces
  for(char c: piecePlacement){

    if(c == '/') continue ;

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

  // Defining the side which will make the move
  sideToMove = (activeColor == "w")? WHITE : BLACK ;

  // Castling rights
  castlingRights = 0 ;
  for(char c : castling){
    if(c == 'K') castlingRights |= 1 ;
    else if(c == 'Q') castlingRights |= 2 ;
    else if(c == 'k') castlingRights |= 4 ;
    else if(c == 'q') castlingRights |= 8 ;
  }

  // En passant square
  if(enPassant == "-") enPassantSquare = -1 ;
  else{
    int file = enPassant[0] - 'a' ;
    int rank = enPassant[1] - '1' ;
    enPassantSquare = squareIndex(rank, file) ;
  }
}

void Board::makeMove(Move move){
  // Save state before doing anything
  history.push_back({ board[move.to], castlingRights, enPassantSquare });
  // Capture moving pieces and reset en-passant
  int piece = board[move.from];
  int pieceType = std::abs(piece);
  enPassantSquare = -1;
  // en - passant
  if(move.flag == EN_PASSANT){
    if(piece > 0){
      board[move.to + 8] = NONE ; //white captures
    }
    else{
      board[move.to - 8] = NONE ; //black captures
    }
  }
  // castling
  else if(move.flag == CASTLING){
    if(move.to == 62){ // white king side
      board[63] = NONE ;
      board[61] = ROOK ;
    }
    else if(move.to == 58){ // white queen side
      board[56] = NONE ;
      board[59] = ROOK ;
    }
    else if(move.to == 6){ // black king side
      board[7] = NONE ;
      board[5] = -ROOK ;
    }
    else if(move.to == 2){ // black queen side
      board[0] = NONE ;
      board[3] = -ROOK ;
    }
  }

  board[move.from] = NONE;
  int sign = (piece > 0) ? 1 : -1;

  if (move.flag == PROMOTE_TO_QUEEN)       board[move.to] = sign * QUEEN;
  else if (move.flag == PROMOTE_TO_ROOK)   board[move.to] = sign * ROOK;
  else if (move.flag == PROMOTE_TO_BISHOP) board[move.to] = sign * BISHOP;
  else if (move.flag == PROMOTE_TO_KNIGHT) board[move.to] = sign * KNIGHT;
  else                                     board[move.to] = piece;

  // Update castling rights
  castlingRights &= castlingRightsMask[move.from];
  castlingRights &= castlingRightsMask[move.to];

  // Update en passant square
  if(pieceType == PAWN && std::abs(move.to - move.from) == 16){
    enPassantSquare = (move.from + move.to) / 2;
  }

  // Switch side to move
  sideToMove = (sideToMove == WHITE) ? BLACK : WHITE;

}

 void Board::unmakeMove(Move move){
  sideToMove = (sideToMove == WHITE) ? BLACK : WHITE;

  UndoState state = history.back();
  history.pop_back();
  castlingRights = state.castlingRights;
  enPassantSquare = state.enPassantSquare;

  int movingPiece;
  if(move.flag == PROMOTE_TO_QUEEN || move.flag == PROMOTE_TO_ROOK || move.flag == PROMOTE_TO_BISHOP || move.flag == PROMOTE_TO_KNIGHT){
    movingPiece = (sideToMove == WHITE) ? PAWN : -PAWN;
  }
  else{
    movingPiece = board[move.to];
  }
  board[move.from] = movingPiece;
  board[move.to] = state.capturedPiece;

  if(move.flag == EN_PASSANT){
    if(sideToMove == WHITE){
      board[move.to + 8] = -PAWN; // Restore captured black pawn
    }
    else{
      board[move.to - 8] = PAWN; // Restore captured white pawn
    }
  }
  else if(move.flag == CASTLING){
    if(move.to == 62){ // white king side
      board[63] = ROOK ;
      board[61] = NONE ;
    }
    else if(move.to == 58){ // white queen side
      board[56] = ROOK ;
      board[59] = NONE ;
    }
    else if(move.to == 6){ // black king side
      board[7] = -ROOK ;
      board[5] = NONE ;
    }
    else if(move.to == 2){ // black queen side
      board[0] = -ROOK ;
      board[3] = NONE ;
    }
  }
}

// Getter Functions
int Board::getPieceAt(int sq) const{
  return board[sq] ;
}

int Board::getSideToMove() const{
  return sideToMove ; 
}

int Board::getEnPassantSquare() const{
  return enPassantSquare ;
}

int Board::getCastlingRights() const{
  return castlingRights ;
}