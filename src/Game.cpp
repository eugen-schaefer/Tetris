#include "Game.h"

#include <chrono>
#include <thread>

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
    auto time_stamp_since_last_update{std::chrono::system_clock::now()};
    long duration_ms_for_one_step_drop{700};
    while (window.isOpen()) {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            };
            m_play_ground.ProcessKeyEvents(event);
        }

        if (!m_play_ground.IsGameOver()) {
            long elapsed_time_since_last_update =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now() -
                    time_stamp_since_last_update)
                    .count();

            // Perfom periodic drop of the active shape
            if (elapsed_time_since_last_update >=
                duration_ms_for_one_step_drop) {
                m_play_ground.MoveActiveShapeOneStepDown();
                time_stamp_since_last_update = std::chrono::system_clock::now();
                if (m_play_ground.IsGameOver()) {
                    // Jump to the begin of the loop to avoid clearing the
                    // window in the following steps
                    continue;
                }
            }

            m_play_ground.ProcessLockDown();

            // Clear screen
            window.clear(sf::Color::White);

            // draw playground
            window.draw(m_play_ground);

            // Update the window
            window.display();
        } else if (m_play_ground.IsGameOver() &&
                   !m_play_ground.IsGameOverAlreadyAnnounced()) {
            window.draw(m_play_ground);
            window.display();
            m_play_ground.SetGameOverAnnounced();
        }
    }
}
