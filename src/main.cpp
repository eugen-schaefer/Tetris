#include <iostream>

#include "Game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(700, 1000), "Tetris");

    Game game(window);
    game.start(window);

    return EXIT_SUCCESS;
}