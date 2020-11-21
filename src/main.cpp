#include "Controller.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(700, 1000), "Tetris");

    sf::Font font;
    if (font.loadFromFile("src/Gasalt-Regular.ttf")) {
        Controller controller(window, font);
        controller.StartGame(window);
    }

    return EXIT_SUCCESS;
}