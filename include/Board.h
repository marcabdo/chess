#pragma once

#include <array>
#include <vector>
#include "Piece.h"
#include "Move.h"
#include <cstdint>

class Board {
public:
    Board();

    Piece getPiece(int square) const;
    void setPiece(int square, Piece p);
    bool isEmpty(int square) const;
    void print() const;
    std::vector<Move> pseudoLegalMoves(Color side) const;
    bool squareAttacked(int square, Color by) const;
    bool kingInCheck(Color side) const;
    std::vector<Move> legalMoves(Color side);
    void makeMove(Move m);
    void undoMove(const Move& m);
    uint64_t perft(int depth);
    uint64_t perftDivide(int depth);
    std::string moveToString(const Move& m);
    bool isCheckmate(Color side) const;
    bool isStalemate(Color side) const;


private:

    std::array<Piece, 64> squares;

    void addKnightMoves(int square, Color side, std::vector<Move>& moves) const;
    void addPawnMoves(int square, Color side, std::vector<Move>& moves) const;
    void addBishopMoves(int square, Color side, std::vector<Move>& moves) const;
    void addRookMoves(int square, Color side, std::vector<Move>& moves) const;
    void addQueenMoves(int square, Color side, std::vector<Move>& moves) const;
    void addKingMoves(int square, Color side, std::vector<Move>& moves) const;

    int lastMoveFrom = -1;
    int lastMoveTo = -1;
    Piece lastMovePiece = {Color::WHITE, PieceType::NONE};

    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteKingsideRookMoved = false;
    bool whiteQueensideRookMoved = false;
    bool blackKingsideRookMoved = false;
    bool blackQueensideRookMoved = false;

    Color sideToMove;
};
