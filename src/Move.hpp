#pragma once
#include <string>

struct Move { 
  int from; 
  int to; 
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
  return s ; 
}
