#pragma once
#include <string>
#include "Types.hpp"

struct Move { 
  int from; 
  int to; 
  int flag = NO_FLAG;
};

inline std::string squareToAlgebraic(int sq){
  if (sq < 0 || sq > 63) return "-";
  int rank = 7 - (sq / 8) ;
  int file = sq % 8 ;
  std::string s = "" ;
  s += (file + 'a') ;
  s += std::to_string(rank + 1) ;
  return s ;
}

inline std::string moveToString(Move m){
  std :: string s = squareToAlgebraic(m.from) + squareToAlgebraic(m.to);
  if(m.flag == PROMOTE_TO_QUEEN){
    s += 'q' ;
  }
  else if(m.flag == PROMOTE_TO_ROOK){
    s += 'r' ;
  }
  else if(m.flag == PROMOTE_TO_BISHOP){
    s += 'b' ;
  }
  else if(m.flag == PROMOTE_TO_KNIGHT){
    s += 'n' ;
  }
  return s ; 
}
