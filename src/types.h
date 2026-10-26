#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <array>
#include <bit>


using Bitboard = uint64_t ;

enum Color: uint8_t {
  WHITE,
  BLACK,
  COLOR_NB = 2
};

enum PieceType : uint8_t {
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
  PIECE_TYPE_NB = 6,
  NO_PIECE = 7
};

enum Square : int8_t {
    SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
    SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
    SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
    SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
    SQ_NONE = -1
};

namespace BitOps{
  inline void setBit(Bitboard& bb , Square sq){
    bb = bb | (1ULL << sq) ;
  }
  inline void clearBit(Bitboard& bb , Square sq){
    bb = bb & ~(1ULL << sq) ;
  }
  inline bool testBit(Bitboard bb , Square sq){
    return (bb & (1ULL << sq)) != 0ULL ;
  }
  inline int countBits(Bitboard bb){
    return std::popcount(bb) ;
  }
  inline Square lsb(Bitboard bb){
    return static_cast<Square>(std::countr_zero(bb)) ;
  }
  inline Square popLsb(Bitboard& bb){
    Square sq = lsb(bb) ;
    bb = bb & (bb - 1) ;
    return sq ;
  }
}
