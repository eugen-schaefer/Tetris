#include "Game.h"

#include "GridGraphic.h"

Game::Game(sf::RenderWindow& window, sf::Font& font)
    : m_play_ground{PlayGround(m_number_rows, m_number_columns, window, font)} {
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
            if (event.type == sf::Event::Closed) {
                window.close();
            };
            m_play_ground.Update(event);
        }
        // Clear screen
        window.clear(sf::Color::White);

        // draw playground
        window.draw(m_play_ground);

        // Update the window
        window.display();
    }
}
