#pragma once

enum class Color{
    WHITE,
    BLACK
};

enum class PieceType{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    NONE
};

struct Piece {
    Color color;
    PieceType type;
};