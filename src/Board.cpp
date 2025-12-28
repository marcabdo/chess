#include <iostream>
#include <cctype>
#include "Board.h"
#include "Piece.h"

Board::Board() {
    //set all squares to empty
    for (auto& square : squares) {
        square = {Color::WHITE, PieceType::NONE};
    }
    //set up white pieces
    squares[0] = {Color::WHITE, PieceType::ROOK};
    squares[1] = {Color::WHITE, PieceType::KNIGHT};
    squares[2] = {Color::WHITE, PieceType::BISHOP};
    squares[3] = {Color::WHITE, PieceType::QUEEN};
    squares[4] = {Color::WHITE, PieceType::KING};
    squares[5] = {Color::WHITE, PieceType::BISHOP};
    squares[6] = {Color::WHITE, PieceType::KNIGHT};
    squares[7] = {Color::WHITE, PieceType::ROOK};
    for (int i = 8; i < 16; ++i) {
        squares[i] = {Color::WHITE, PieceType::PAWN};
    }
    //set up black pieces
    squares[56] = {Color::BLACK, PieceType::ROOK};
    squares[57] = {Color::BLACK, PieceType::KNIGHT};
    squares[58] = {Color::BLACK, PieceType::BISHOP};
    squares[59] = {Color::BLACK, PieceType::QUEEN};
    squares[60] = {Color::BLACK, PieceType::KING};
    squares[61] = {Color::BLACK, PieceType::BISHOP};
    squares[62] = {Color::BLACK, PieceType::KNIGHT};
    squares[63] = {Color::BLACK, PieceType::ROOK};
    for (int i = 48; i < 56; ++i) {
        squares[i] = {Color::BLACK, PieceType::PAWN};
    }

    sideToMove = Color::WHITE;
}
Piece Board::getPiece(int square) const {
    if (square < 0 || square >= 64) {
        std::cerr << "Error: getPiece called with invalid square index " << square << std::endl;
        return {Color::WHITE, PieceType::NONE};
    }
    return squares[square];
}
void Board::setPiece(int square, Piece p) {
    if (square < 0 || square >= 64) {
        std::cerr << "Error: setPiece called with invalid square index " << square << std::endl;
        return;
    }
    squares[square] = p;
}
bool Board::isEmpty(int square) const {
    if (square < 0 || square >= 64) {
        std::cerr << "Error: isEmpty called with invalid square index " << square << std::endl;
        return true;
    }
    return squares[square].type == PieceType::NONE;
}
void Board::print() const {

    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            const Piece& piece = squares[square];

            char symbol = '.';
            if (piece.type != PieceType::NONE) {
                switch (piece.type) {
                    case PieceType::PAWN:   symbol = 'P'; break;
                    case PieceType::ROOK:   symbol = 'R'; break;
                    case PieceType::KNIGHT: symbol = 'N'; break;
                    case PieceType::BISHOP: symbol = 'B'; break;
                    case PieceType::QUEEN:  symbol = 'Q'; break;
                    case PieceType::KING:   symbol = 'K'; break;
                    default: break;
                }
                if (piece.color == Color::BLACK) {
                    symbol = std::tolower(symbol);
                }
            }
            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
}

std::vector<Move> Board::pseudoLegalMoves(Color side) const {
    std::vector<Move> moves;

    for (int square = 0; square < 64; ++square) {
        Piece p = getPiece(square);
        if (p.color != side)
            continue;

        switch (p.type) {
            case PieceType::PAWN:
                addPawnMoves(square, side, moves);
                break;
            case PieceType::KNIGHT:
                addKnightMoves(square, side, moves);
                break;
            case PieceType::BISHOP:
                addBishopMoves(square, side, moves);
                break;
            case PieceType::ROOK:
                addRookMoves(square, side, moves);
                break;
            case PieceType::QUEEN:
                addQueenMoves(square, side, moves);
                break;
            case PieceType::KING:
                addKingMoves(square, side, moves);
                break;
            default:
                break;
        }
    }
    return moves;
}

void Board::addKnightMoves(int square, Color side, std::vector<Move>& moves) const {
    static const int knightOffsets[8] = {15, 17, 6, 10, -15, -17, -6, -10};

    int fromRank = square / 8;
    int fromFile = square % 8;

    for (int offset : knightOffsets) {
        int target = square + offset;
        if (target < 0 || target >= 64)
            continue;

        int toRank = target / 8;
        int toFile = target % 8;

        int dr = std::abs(fromRank - toRank);
        int df = std::abs(fromFile - toFile);

        if (!((dr == 2 && df == 1) || (dr == 1 && df == 2)))
            continue;

        Piece targetPiece = getPiece(target);
        if (targetPiece.type == PieceType::NONE || targetPiece.color != side) {
            Move m;
            m.from = square;
            m.to = target;
            m.captured = targetPiece;
            moves.push_back(m);
        }
    }
}

void Board::addPawnMoves(int square, Color side, std::vector<Move>& moves) const {
    if (side == Color::WHITE){

        int rank = square / 8;
        int file = square % 8;

        int forward = square + 8;
        if (forward < 64 && isEmpty(forward)) {
            Move m;
            m.from = square;
            m.to = forward;
            m.captured = Piece{Color::WHITE, PieceType::NONE};
            if (m.to/8 == 7) {
                m.promotion = true;
            }
            moves.push_back(m);

            if (rank == 1) {
                int doubleForward = square + 16;
                if (doubleForward < 64 && isEmpty(doubleForward)) {
                    Move m2;
                    m2.from = square;
                    m2.to = doubleForward;
                    m2.captured = Piece{Color::WHITE, PieceType::NONE};
                    moves.push_back(m2);
                }
            }
        }

        if (file > 0) {
            int captureLeft = square + 7;
            if (captureLeft < 64) {
                Piece target = getPiece(captureLeft);
                if (target.type != PieceType::NONE && target.color == Color::BLACK) {
                    Move m;
                    m.from = square;
                    m.to = captureLeft;
                    m.captured = target;
                    if (m.to/8 == 7) {
                        m.promotion = true;
                    }
                    moves.push_back(m);
                }
            }
        }

        if (file < 7) {
            int captureRight = square + 9;
            if (captureRight < 64) {
                Piece target = getPiece(captureRight);
                if (target.type != PieceType::NONE && target.color == Color::BLACK) {
                    Move m;
                    m.from = square;
                    m.to = captureRight;
                    m.captured = target;
                    if (m.to/8 == 7) {
                        m.promotion = true;
                    }
                    moves.push_back(m);
                }
            }
        }
        // En passant (WHITE)
        if (rank == 4) { // pawn must be on 5th rank
            // Enemy pawn moved last turn
            if (lastMovePiece.type == PieceType::PAWN &&
                lastMovePiece.color == Color::BLACK &&
                lastMoveFrom / 8 == 6 &&
                lastMoveTo / 8 == 4) {

                // Pawn is to the left
                if (file > 0 && lastMoveTo == square - 1) {
                    Move m;
                    m.from = square;
                    m.to = square + 7;
                    m.enPassant = true;
                    m.captured = lastMovePiece;
                    moves.push_back(m);
                }

                // Pawn is to the right
                if (file < 7 && lastMoveTo == square + 1) {
                    Move m;
                    m.from = square;
                    m.to = square + 9;
                    m.enPassant = true;
                    m.captured = lastMovePiece;
                    moves.push_back(m);
                }
            }
        }

    }
    else { 
        int rank = square / 8;
        int file = square % 8;

        int forward = square - 8;
        if (forward >= 0 && isEmpty(forward)) {
            Move m;
            m.from = square;
            m.to = forward;
            m.captured = Piece{Color::BLACK, PieceType::NONE};
            if (m.to/8 == 0) {
                m.promotion = true;
            }
            moves.push_back(m);

            if (rank == 6) {
                int doubleForward = square - 16;
                if (doubleForward >= 0 && isEmpty(doubleForward)) {
                    Move m2;
                    m2.from = square;
                    m2.to = doubleForward;
                    m2.captured = Piece{Color::BLACK, PieceType::NONE};
                    moves.push_back(m2);
                }
            }
        }

        if (file > 0) {
            int captureLeft = square - 9;
            if (captureLeft >= 0) {
                Piece target = getPiece(captureLeft);
                if (target.type != PieceType::NONE && target.color == Color::WHITE) {
                    Move m;
                    m.from = square;
                    m.to = captureLeft;
                    m.captured = target;
                    if (m.to/8 == 0) {
                        m.promotion = true;
                    }
                    moves.push_back(m);
                }
            }
        }

        if (file < 7) {
            int captureRight = square - 7;
            if (captureRight >= 0) {
                Piece target = getPiece(captureRight);
                if (target.type != PieceType::NONE && target.color == Color::WHITE) {
                    Move m;
                    m.from = square;
                    m.to = captureRight;
                    m.captured = target;
                    if (m.to/8 == 0) {
                        m.promotion = true;
                    }
                    moves.push_back(m);
                }
            }
        }
        // En passant (BLACK)
        if (rank == 3) { // pawn must be on 4th rank
            if (lastMovePiece.type == PieceType::PAWN &&
                lastMovePiece.color == Color::WHITE &&
                lastMoveFrom / 8 == 1 &&
                lastMoveTo / 8 == 3) {

                if (file > 0 && lastMoveTo == square - 1) {
                    Move m;
                    m.from = square;
                    m.to = square - 9;
                    m.enPassant = true;
                    m.captured = lastMovePiece;
                    moves.push_back(m);
                }

                if (file < 7 && lastMoveTo == square + 1) {
                    Move m;
                    m.from = square;
                    m.to = square - 7;
                    m.enPassant = true;
                    m.captured = lastMovePiece;
                    moves.push_back(m);
                }
            }
        }

    }
}


void Board::addBishopMoves(int square, Color side, std::vector<Move>& moves) const {
    const int directions[4] = { 9, 7, -7, -9 };

    for (int dir : directions) {
        int current = square;

        while (true) {
            int next = current + dir;

            if (next < 0 || next >= 64)
                break;

            if (std::abs((next % 8) - (current % 8)) != 1)
                break;

            Piece target = getPiece(next);

            if (target.type == PieceType::NONE) {
                moves.push_back({ square, next, Piece{Color::WHITE, PieceType::NONE} });
            } else {
                if (target.color != side) {
                    moves.push_back({ square, next, target });
                }
                break;
            }

            current = next;
        }
    }
}


void Board:: addRookMoves(int square, Color side, std::vector<Move>& moves) const {
    const int directions[4] = { 8, -8, 1, -1 };

    for (int dir : directions) {
        int current = square;

        while (true) {
            int next = current + dir;

            if (next < 0 || next >= 64)
                break;

            if ((dir == 1 || dir == -1) && (next / 8 != current / 8))
                break;

            Piece target = getPiece(next);

            if (target.type == PieceType::NONE) {
                moves.push_back({ square, next, Piece{Color::WHITE, PieceType::NONE} });
            } else {
                if (target.color != side) {
                    moves.push_back({ square, next, target });
                }
                break;
            }

            current = next;
        }
    }
}

void Board::addQueenMoves(int square, Color side, std::vector<Move>& moves) const {
    addBishopMoves(square, side, moves);
    addRookMoves(square, side, moves);
}

void Board::addKingMoves(int square, Color side, std::vector<Move>& moves) const {
    static const int kingOffsets[8] = {8, 7, 9, -8, -7, -9, 1, -1};

    int fromRank = square / 8;
    int fromFile = square % 8;

    for (int offset : kingOffsets) {
        int target = square + offset;
        if (target < 0 || target >= 64)
            continue;

        int toRank = target / 8;
        int toFile = target % 8;

        int dr = std::abs(fromRank - toRank);
        int df = std::abs(fromFile - toFile);

        if (dr > 1 || df > 1)
            continue;

        Piece targetPiece = getPiece(target);
        if (targetPiece.type == PieceType::NONE || targetPiece.color != side) {
            Move m;
            m.from = square;
            m.to = target;
            m.captured = targetPiece;
            moves.push_back(m);
        }
    }
    // Castling
    if (side == Color::WHITE && !whiteKingMoved && square == 4) {
        // King-side
        if (!whiteKingsideRookMoved &&
            getPiece(7).type == PieceType::ROOK &&
            getPiece(7).color == Color::WHITE &&
            isEmpty(5) && isEmpty(6) &&
            !squareAttacked(4, Color::BLACK) &&
            !squareAttacked(5, Color::BLACK) &&
            !squareAttacked(6, Color::BLACK)) {

            Move m;
            m.from = 4;
            m.to = 6;
            m.castling = true;
            moves.push_back(m);
        }

        // Queen-side
        if (!whiteQueensideRookMoved &&
            getPiece(0).type == PieceType::ROOK &&
            getPiece(0).color == Color::WHITE &&
            isEmpty(1) && isEmpty(2) && isEmpty(3) &&
            !squareAttacked(4, Color::BLACK) &&
            !squareAttacked(3, Color::BLACK) &&
            !squareAttacked(2, Color::BLACK)) {

            Move m;
            m.from = 4;
            m.to = 2;
            m.castling = true;
            moves.push_back(m);
        }
    }

    else if (side == Color::BLACK && !blackKingMoved && square == 60) {
        // King-side
        if (!blackKingsideRookMoved &&
            getPiece(63).type == PieceType::ROOK &&
            getPiece(63).color == Color::BLACK &&
            isEmpty(61) && isEmpty(62) &&
            !squareAttacked(60, Color::WHITE) &&
            !squareAttacked(61, Color::WHITE) &&
            !squareAttacked(62, Color::WHITE)) {

            Move m;
            m.from = 60;
            m.to = 62;
            m.castling = true;
            moves.push_back(m);
        }

        // Queen-side
        if (!blackQueensideRookMoved &&
            getPiece(56).type == PieceType::ROOK &&
            getPiece(56).color == Color::BLACK &&
            isEmpty(57) && isEmpty(58) && isEmpty(59) &&
            !squareAttacked(60, Color::WHITE) &&
            !squareAttacked(59, Color::WHITE) &&
            !squareAttacked(58, Color::WHITE)) {

            Move m;
            m.from = 60;
            m.to = 58;
            m.castling = true;
            moves.push_back(m);
        }
    }

}

bool Board::squareAttacked(int square, Color by) const {
    int rank = square / 8;
    int file = square % 8;

    // ===== PAWN ATTACKS =====
    if (by == Color::WHITE) {
        if (file > 0 && square - 9 >= 0) {
            Piece p = getPiece(square - 9);
            if (p.type == PieceType::PAWN && p.color == Color::WHITE)
                return true;
        }
        if (file < 7 && square - 7 >= 0) {
            Piece p = getPiece(square - 7);
            if (p.type == PieceType::PAWN && p.color == Color::WHITE)
                return true;
        }
    } else {
        if (file > 0 && square + 7 < 64) {
            Piece p = getPiece(square + 7);
            if (p.type == PieceType::PAWN && p.color == Color::BLACK)
                return true;
        }
        if (file < 7 && square + 9 < 64) {
            Piece p = getPiece(square + 9);
            if (p.type == PieceType::PAWN && p.color == Color::BLACK)
                return true;
        }
    }

    // ===== KNIGHTS =====
    static const int knightOffsets[8] = {15, 17, 6, 10, -15, -17, -6, -10};
    for (int off : knightOffsets) {
        int sq = square + off;
        if (sq >= 0 && sq < 64) {
            Piece p = getPiece(sq);
            if (p.type == PieceType::KNIGHT && p.color == by)
                return true;
        }
    }

    // ===== BISHOPS / QUEENS (diagonals) =====
    const int diagDirs[4] = {9, 7, -7, -9};
    for (int dir : diagDirs) {
        int cur = square;
        while (true) {
            int nxt = cur + dir;
            if (nxt < 0 || nxt >= 64 ||
                std::abs((nxt % 8) - (cur % 8)) != 1)
                break;

            Piece p = getPiece(nxt);
            if (p.type != PieceType::NONE) {
                if ((p.type == PieceType::BISHOP || p.type == PieceType::QUEEN)
                    && p.color == by)
                    return true;
                break;
            }
            cur = nxt;
        }
    }

    // ===== ROOKS / QUEENS (straight) =====
    const int straightDirs[4] = {8, -8, 1, -1};
    for (int dir : straightDirs) {
        int cur = square;
        while (true) {
            int nxt = cur + dir;
            if (nxt < 0 || nxt >= 64 ||
                ((dir == 1 || dir == -1) && (nxt / 8 != cur / 8)))
                break;

            Piece p = getPiece(nxt);
            if (p.type != PieceType::NONE) {
                if ((p.type == PieceType::ROOK || p.type == PieceType::QUEEN)
                    && p.color == by)
                    return true;
                break;
            }
            cur = nxt;
        }
    }

    // ===== KING =====
    static const int kingOffsets[8] = {8,7,9,-8,-7,-9,1,-1};
    for (int off : kingOffsets) {
        int sq = square + off;
        if (sq >= 0 && sq < 64) {
            Piece p = getPiece(sq);
            if (p.type == PieceType::KING && p.color == by)
                return true;
        }
    }

    return false;
}



bool Board::kingInCheck(Color side) const {
    int kingSquare = -1;
    for (int i = 0; i < 64; ++i) {
        Piece p = getPiece(i);
        if (p.type == PieceType::KING && p.color == side) {
            kingSquare = i;
            break;
        }
    }

    // 🚨 SAFETY CHECK
    if (kingSquare == -1) {
        std::cerr << "Error: king not found for side "
                  << (side == Color::WHITE ? "WHITE" : "BLACK") << std::endl;
        return false;
    }

    Color opponent = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return squareAttacked(kingSquare, opponent);
}


std::vector<Move> Board::legalMoves(Color side) {
    std::vector<Move> legalMoves;
    auto moves = pseudoLegalMoves(side);

    for (auto move : moves) {           // NOT const (makeMove modifies Move)
        Board tempboard = *this;

        // Apply the move using full rules
        tempboard.makeMove(move);

        // Keep move only if king is safe
        if (!tempboard.kingInCheck(side)) {
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}



void Board::makeMove(Move m) {
    Piece movingPiece = getPiece(m.from);
    Piece empty = { Color::WHITE, PieceType::NONE };

    // --- Save state for undo ---
    m.prevWhiteKingMoved = whiteKingMoved;
    m.prevBlackKingMoved = blackKingMoved;
    m.prevWhiteKSrookMoved = whiteKingsideRookMoved;
    m.prevWhiteQSrookMoved = whiteQueensideRookMoved;
    m.prevBlackKSrookMoved = blackKingsideRookMoved;
    m.prevBlackQSrookMoved = blackQueensideRookMoved;

    m.prevLastMoveFrom = lastMoveFrom;
    m.prevLastMoveTo = lastMoveTo;
    m.prevLastMovePiece = lastMovePiece;

    // --- Update castling rights based on moving piece ---
    if (movingPiece.type == PieceType::KING) {
        if (movingPiece.color == Color::WHITE)
            whiteKingMoved = true;
        else
            blackKingMoved = true;
    }

    if (movingPiece.type == PieceType::ROOK) {
        if (movingPiece.color == Color::WHITE) {
            if (m.from == 7) whiteKingsideRookMoved = true;
            if (m.from == 0) whiteQueensideRookMoved = true;
        } else {
            if (m.from == 63)  blackKingsideRookMoved = true;
            if (m.from == 56)  blackQueensideRookMoved = true;
        }
    }

    // --- If a rook is captured, lose castling rights ---
    if (m.captured.type == PieceType::ROOK) {
        if (m.captured.color == Color::WHITE) {
            if (m.to == 7) whiteKingsideRookMoved = true;
            if (m.to == 0) whiteQueensideRookMoved = true;
        } else {
            if (m.to == 63) blackKingsideRookMoved = true;
            if (m.to == 56) blackQueensideRookMoved = true;
        }
    }

    // --- Apply the move ---
    if (m.promotion) {
        setPiece(m.to, { movingPiece.color, PieceType::QUEEN });
        setPiece(m.from, empty);
    }
    else if (m.enPassant) {
        setPiece(m.to, movingPiece);
        setPiece(m.from, empty);

        int capSq = (movingPiece.color == Color::WHITE)
                    ? m.to - 8
                    : m.to + 8;

        setPiece(capSq, empty);
    }
    else if (m.castling) {
        setPiece(m.to, movingPiece);
        setPiece(m.from, empty);

        // --- WHITE CASTLING ---
        if (movingPiece.color == Color::WHITE) {
            // King-side: e1 → g1
            if (m.to == 6) {
                whiteKingsideRookMoved = true;
                setPiece(5, getPiece(7));   // rook h1 → f1
                setPiece(7, empty);
            }
            // Queen-side: e1 → c1
            else if (m.to == 2) {
                whiteQueensideRookMoved = true;
                setPiece(3, getPiece(0));   // rook a1 → d1
                setPiece(0, empty);
            }
        }

        // --- BLACK CASTLING ---
        else {
            // King-side: e8 → g8
            if (m.to == 62) {
                blackKingsideRookMoved = true;
                setPiece(61, getPiece(63)); // rook h8 → f8
                setPiece(63, empty);
            }
            // Queen-side: e8 → c8
            else if (m.to == 58) {
                blackQueensideRookMoved = true;
                setPiece(59, getPiece(56)); // rook a8 → d8
                setPiece(56, empty);
            }
        }
    }
    else {
        setPiece(m.to, movingPiece);
        setPiece(m.from, empty);
    }

    // --- Update last-move info ---
    lastMoveFrom = m.from;
    lastMoveTo = m.to;
    lastMovePiece = movingPiece;

    sideToMove = (sideToMove == Color::WHITE) ? Color::BLACK : Color::WHITE;
}



void Board::undoMove(const Move& m) {
    Piece empty = { Color::WHITE, PieceType::NONE };

    // --- Restore last-move info ---
    lastMoveFrom  = m.prevLastMoveFrom;
    lastMoveTo    = m.prevLastMoveTo;
    lastMovePiece = m.prevLastMovePiece;

    // --- Restore castling rights ---
    whiteKingMoved = m.prevWhiteKingMoved;
    blackKingMoved = m.prevBlackKingMoved;
    whiteKingsideRookMoved = m.prevWhiteKSrookMoved;
    whiteQueensideRookMoved = m.prevWhiteQSrookMoved;
    blackKingsideRookMoved = m.prevBlackKSrookMoved;
    blackQueensideRookMoved = m.prevBlackQSrookMoved;

    // --- Undo special moves ---
    if (m.castling) {
        // Move king back
        setPiece(m.from, getPiece(m.to));
        setPiece(m.to, empty);

        // Move rook back
        if (m.to == 6) {          // White king-side
            setPiece(7, getPiece(5));
            setPiece(5, empty);
        }
        else if (m.to == 2) {     // White queen-side
            setPiece(0, getPiece(3));
            setPiece(3, empty);
        }
        else if (m.to == 62) {      // Black king-side
            setPiece(63, getPiece(61));
            setPiece(61, empty);
        }
        else if (m.to == 58) {      // Black queen-side
            setPiece(56, getPiece(59));
            setPiece(59, empty);
        }
    }
    else if (m.enPassant) {
        // Restore pawn
        Piece pawn = getPiece(m.to);
        setPiece(m.from, pawn);
        setPiece(m.to, empty);

        int capSq = (pawn.color == Color::WHITE)
                    ? m.to - 8
                    : m.to + 8;

        setPiece(capSq, m.captured);
    }
    else if (m.promotion) {
        // Restore pawn
        Piece promoted = getPiece(m.to);
        setPiece(m.from, { promoted.color, PieceType::PAWN });
        setPiece(m.to, m.captured);
    }
    else {
        // Normal move
        setPiece(m.from, getPiece(m.to));
        setPiece(m.to, m.captured);
    }

    sideToMove = (sideToMove == Color::WHITE) ? Color::BLACK : Color::WHITE;
}


uint64_t Board::perft(int depth) {
    if (depth == 0)
        return 1;

    uint64_t nodes = 0;
    auto moves = legalMoves(sideToMove);

    for (auto move : moves) {
        makeMove(move);

        nodes += perft(depth - 1);

        undoMove(move);
    }

    return nodes;
}


uint64_t Board::perftDivide(int depth) {
    uint64_t total = 0;
    auto moves = legalMoves(sideToMove);

    for (auto move : moves) {
        makeMove(move);

        uint64_t count = perft(depth - 1);

        undoMove(move);

        std::cout << moveToString(move) << ": " << count << "\n";
        total += count;
    }

    std::cout << "Total: " << total << std::endl;
    return total;
}


std::string Board::moveToString(const Move& m) {
    auto sqToStr = [](int sq) {
        char file = 'a' + (sq % 8);
        char rank = '1' + (sq / 8);
        return std::string{file, rank};
    };

    std::string s = sqToStr(m.from) + sqToStr(m.to);

    if (m.promotion)
        s += "q"; // queen promotion

    return s;
}

