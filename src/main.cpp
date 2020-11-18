#include "Game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(700, 1000), "Tetris");

    sf::Font font;
    if (font.loadFromFile("src/Gasalt-Regular.ttf")) {
        Game game(window, font);
        game.start(window);
    }

    return EXIT_SUCCESS;
}