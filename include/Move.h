#pragma once

#include "Piece.h"

struct Move {
    int from;
    int to;
    Piece captured;

    bool promotion = false;
    bool castling = false;
    bool enPassant = false;

    bool prevWhiteKingMoved;
    bool prevBlackKingMoved;
    bool prevWhiteKSrookMoved;
    bool prevWhiteQSrookMoved;
    bool prevBlackKSrookMoved;
    bool prevBlackQSrookMoved;

    int prevLastMoveFrom;
    int prevLastMoveTo;
    Piece prevLastMovePiece;
};
