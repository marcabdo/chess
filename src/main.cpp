#include <iostream>
#include <string>
#include "Board.h"
#include "Move.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <stdexcept>



enum class GameMode {
    NONE,
    FRIEND,
    COMPUTER
};

GameMode runMenu();
void runGameVsFriend();
void runUnderDevelopmentWindow();


/*
int algebraicToSquare(const std::string& s) {
    int file = s[0] - 'a';   // a..h -> 0..7
    int rank = s[1] - '1';   // 1..8 -> 0..7
    return rank * 8 + file;
}
*/



/*
int main(){
    Board board;

    std::cout << board.perft(1) << std::endl; // 20
    std::cout << board.perft(2) << std::endl; // 400
    std::cout << board.perft(3) << std::endl; // 8902
    std::cout << board.perft(4) << std::endl; // 197281
    std::cout << board.perft(5) << std::endl; // 4865609
    return 0;
}
*/
/*
int main() {
    Board board;
    Color sideToMove = Color::WHITE;

    std::cout << "Chess engine started successfully!\n";

    while (true) {
        board.print();

        // Generate legal moves
        auto legal = board.legalMoves(sideToMove);

        // --- Game over detection ---
        if (legal.empty()) {
            if (board.kingInCheck(sideToMove)) {
                std::cout << "Checkmate! ";
                std::cout << (sideToMove == Color::WHITE ? "Black" : "White")
                          << " wins.\n";
            } else {
                std::cout << "Stalemate!\n";
            }
            break;
        }

        // --- Input ---
        std::string fromStr, toStr;
        std::cout << (sideToMove == Color::WHITE ? "White" : "Black")
                  << " to move (e2 e4): ";

        std::cin >> fromStr >> toStr;

        int from = algebraicToSquare(fromStr);
        int to   = algebraicToSquare(toStr);

        // --- Validate move ---
        bool found = false;
        Move chosenMove;

        for (auto& m : legal) {
            if (m.from == from && m.to == to) {
                chosenMove = m;
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "Illegal move. Try again.\n";
            continue;
        }

        // --- Apply move ---
        board.makeMove(chosenMove);

        // --- Switch side ---
        sideToMove = (sideToMove == Color::WHITE)
                     ? Color::BLACK
                     : Color::WHITE;
    }

    return 0;
}
*/



constexpr int TILE_SIZE  = 128;
constexpr int BOARD_SIZE = TILE_SIZE * 8;

using PieceKey = std::pair<PieceType, Color>;


// Convert board square (0–63) → pixel position
sf::Vector2f squareToPixel(int square) {
    int file = square % 8;
    int rank = square / 8;

    int flippedRank = 7 - rank;

    return sf::Vector2f(
        file * TILE_SIZE,
        flippedRank * TILE_SIZE
    );
}



int pixelToSquare(sf::Vector2i pos) {
    if (pos.x < 0 || pos.y < 0) return -1;

    int file = pos.x / TILE_SIZE;
    int rank = 7 - (pos.y / TILE_SIZE);

    if (file < 0 || file > 7 || rank < 0 || rank > 7)
        return -1;

    return rank * 8 + file;
}




// Load one piece texture
void loadPiece(std::map<PieceKey, sf::Texture>& textures,
               PieceType type,
               Color color,
               const std::string& path) {

    sf::Texture tex;
    if (!tex.loadFromFile(path)) {
        throw std::runtime_error("Failed to load: " + path);
    }
    textures[{type, color}] = std::move(tex);
}


// Draw all pieces
void drawPieces(sf::RenderWindow& window,
                Board& board,
                std::map<PieceKey, sf::Texture>& textures) {

    for (int sq = 0; sq < 64; ++sq) {
        Piece piece = board.getPiece(sq);
        if (piece.type == PieceType::NONE) continue;

        const sf::Texture& tex =
            textures.at({piece.type, piece.color});

        sf::Sprite sprite(tex);

        sf::Vector2u size = tex.getSize();
        float scaleX = TILE_SIZE / static_cast<float>(size.x);
        float scaleY = TILE_SIZE / static_cast<float>(size.y);
        sprite.setScale(sf::Vector2f(scaleX, scaleY));

        sprite.setPosition(squareToPixel(sq));
        window.draw(sprite);
    }
}


int main() {
    GameMode mode = runMenu();

    if (mode == GameMode::FRIEND) {
        runGameVsFriend();
    }
    else if (mode == GameMode::COMPUTER) {
        runUnderDevelopmentWindow();
    }

    return 0;
}


GameMode runMenu() {
    sf::RenderWindow menu(
        sf::VideoMode({400, 300}),
        "Chess - Menu",
        sf::Style::Titlebar | sf::Style::Close
    );

    sf::Font font;
    if (!font.openFromFile("fonts/font.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    sf::Text title(font,"Chess", 36);
    title.setPosition({150.f, 20.f});

    sf::RectangleShape friendBtn({200, 50});
    friendBtn.setPosition({100.f, 100.f});
    friendBtn.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape computerBtn({200, 50});
    computerBtn.setPosition({100.f, 170.f});
    computerBtn.setFillColor(sf::Color(200, 200, 200));

    sf::Text friendText(font, "Play vs Friend", 20);
    friendText.setPosition({120.f, 110.f});
    sf::Text computerText(font, "Play vs Computer", 20);
    computerText.setPosition({105.f, 180.f});

    while (menu.isOpen()) {
        while (auto event = menu.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                menu.close();
                return GameMode::NONE;
            }

            if (const auto* mouse =
                event->getIf<sf::Event::MouseButtonPressed>()) {

                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mpos =
                        sf::Mouse::getPosition(menu);

                    if (friendBtn.getGlobalBounds().contains(
                            sf::Vector2f{(float)mpos.x, (float)mpos.y})) {
                        menu.close();
                        return GameMode::FRIEND;
                    }

                    if (computerBtn.getGlobalBounds().contains(
                            sf::Vector2f{(float)mpos.x, (float)mpos.y})) {
                        menu.close();
                        return GameMode::COMPUTER;
                    }
                }
            }
        }

        menu.clear(sf::Color::White);
        menu.draw(title);
        menu.draw(friendBtn);
        menu.draw(computerBtn);
        menu.draw(friendText);
        menu.draw(computerText);
        menu.display();
    }

    return GameMode::NONE;
}



void runUnderDevelopmentWindow() {
    sf::RenderWindow window(
        sf::VideoMode({400, 200}),
        "Under Development",
        sf::Style::Titlebar | sf::Style::Close
    );

    sf::Font font;
    if (!font.openFromFile("fonts/font.ttf")) {
        throw std::runtime_error("Failed to load fonts/font.ttf");
    }

    sf::Text msg(
        font,
        "Computer opponent\nis under development",
        22
    );
    msg.setPosition({40.f, 60.f});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(msg);
        window.display();
    }
}



void runGameVsFriend() {
    // ================= WINDOW =================
    sf::RenderWindow window(
        sf::VideoMode({1024, 1024}),
        "Chess",
        sf::Style::Titlebar | sf::Style::Close
    );

    sf::Vector2u winSize = window.getSize();
    const int TILE_SIZE = winSize.x / 8;

    Board board;
    int selectedSquare = -1;
    Color sideToMove = Color::WHITE;

    std::vector<Move> selectedMoves;

    // ================= BOARD =================
    sf::Texture boardTexture;
    if (!boardTexture.loadFromFile("images/board.png")) {
        std::cerr << "Failed to load board.png\n";
        return;
    }

    sf::Sprite boardSprite(boardTexture);
    boardSprite.setScale(sf::Vector2f(
        winSize.x / float(boardTexture.getSize().x),
        winSize.y / float(boardTexture.getSize().y)
    ));

    // ================= PIECES =================
    std::map<PieceKey, sf::Texture> pieceTextures;

    loadPiece(pieceTextures, PieceType::PAWN,   Color::WHITE, "images/pieces/whitePawn.png");
    loadPiece(pieceTextures, PieceType::KNIGHT, Color::WHITE, "images/pieces/whiteKnight.png");
    loadPiece(pieceTextures, PieceType::BISHOP, Color::WHITE, "images/pieces/whiteBishop.png");
    loadPiece(pieceTextures, PieceType::ROOK,   Color::WHITE, "images/pieces/whiteRook.png");
    loadPiece(pieceTextures, PieceType::QUEEN,  Color::WHITE, "images/pieces/whiteQueen.png");
    loadPiece(pieceTextures, PieceType::KING,   Color::WHITE, "images/pieces/whiteKing.png");

    loadPiece(pieceTextures, PieceType::PAWN,   Color::BLACK, "images/pieces/blackPawn.png");
    loadPiece(pieceTextures, PieceType::KNIGHT, Color::BLACK, "images/pieces/blackKnight.png");
    loadPiece(pieceTextures, PieceType::BISHOP, Color::BLACK, "images/pieces/blackBishop.png");
    loadPiece(pieceTextures, PieceType::ROOK,   Color::BLACK, "images/pieces/blackRook.png");
    loadPiece(pieceTextures, PieceType::QUEEN,  Color::BLACK, "images/pieces/blackQueen.png");
    loadPiece(pieceTextures, PieceType::KING,   Color::BLACK, "images/pieces/blackKing.png");

    // ================= MAIN LOOP =================
    while (window.isOpen()) {

        // -------- EVENTS --------
        while (auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouse =
                    event->getIf<sf::Event::MouseButtonPressed>()) {

                if (mouse->button == sf::Mouse::Button::Left) {

                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int clickedSquare = pixelToSquare(mousePos);
                    if (clickedSquare == -1) continue;

                    Piece clickedPiece = board.getPiece(clickedSquare);

                    // ---- SELECT ----
                    if (selectedSquare == -1) {
                        if (clickedPiece.type != PieceType::NONE &&
                            clickedPiece.color == sideToMove) {

                            selectedSquare = clickedSquare;

                            selectedMoves.clear();
                            auto legal = board.legalMoves(sideToMove);
                            for (const auto& m : legal) {
                                if (m.from == selectedSquare) {
                                    selectedMoves.push_back(m);
                                }
                            }
                        }
                    }
                    // ---- MOVE ----
                    else {
                        for (auto& m : selectedMoves) {   // <-- REMOVE const
                            if (m.to == clickedSquare) {
                                board.makeMove(m);
                                sideToMove =
                                    (sideToMove == Color::WHITE)
                                    ? Color::BLACK
                                    : Color::WHITE;
                                break;
                            }
                        }


                        selectedSquare = -1;
                        selectedMoves.clear();
                    }
                }
            }
        }

        // -------- RENDER --------
        window.clear();
        window.draw(boardSprite);

        // Highlight selected square
        if (selectedSquare != -1) {
            sf::RectangleShape highlight(
                sf::Vector2f(TILE_SIZE, TILE_SIZE)
            );
            highlight.setFillColor(sf::Color(255, 255, 0, 100));
            highlight.setPosition(squareToPixel(selectedSquare));
            window.draw(highlight);
        }

        // Highlight available moves
        for (auto& m : selectedMoves) {
            sf::RectangleShape moveHighlight(
                sf::Vector2f(TILE_SIZE, TILE_SIZE)
            );

            Piece target = board.getPiece(m.to);
            if (target.type != PieceType::NONE) {
                moveHighlight.setFillColor(sf::Color(255, 0, 0, 120));
            } else {
                moveHighlight.setFillColor(sf::Color(0, 255, 0, 120));
            }

            moveHighlight.setPosition(squareToPixel(m.to));
            window.draw(moveHighlight);
        }

        // Draw pieces
        for (int sq = 0; sq < 64; ++sq) {
            Piece p = board.getPiece(sq);
            if (p.type == PieceType::NONE) continue;

            PieceKey key{ p.type, p.color };
            sf::Sprite sprite(pieceTextures.at(key));

            auto texSize = sprite.getTexture().getSize();
            sprite.setScale(sf::Vector2f(
                TILE_SIZE / float(texSize.x),
                TILE_SIZE / float(texSize.y)
            ));

            sprite.setPosition(squareToPixel(sq));
            window.draw(sprite);
        }

        window.display();
    }
    return;
}
