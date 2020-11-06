#include "Game.h"

Game::Game(sf::RenderWindow& window)
    : m_grid{GridGraphic(20, 10, 0.1, window.getSize())} {
    // Center the main window
    auto desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2<int> new_position{
        static_cast<int>((desktop.width) / 2 - window.getSize().x / 2),
        static_cast<int>(desktop.height / 2 - window.getSize().y / 2)};
    window.setPosition(new_position);
}

void Game::start(sf::RenderWindow& window) {
    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) window.close();
        }

        // Clear screen
        window.clear(sf::Color::White);

        window.draw(m_grid);

        // Update the window
        window.display();
    }
}
