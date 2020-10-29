#include "Tetromino.h"

Tetromino::Tetromino(IGameBackground& game_background,
                     TetrominoPositionType init_position, Color color)
    : m_game_background{game_background},
      m_position{init_position},
      m_color{color} {};

Color Tetromino::getColor() { return m_color; }

TetrominoPositionType Tetromino::getPosition() { return m_position; }

void Tetromino::setPosition(TetrominoPositionType position) {
    m_position = position;
}

void Tetromino::moveOneStep(Direction direction) {
    TetrominoPositionType tetromino_position = getPosition();
    switch (direction) {
        case Direction::down:
            for (auto& square_position : tetromino_position) {
                ++square_position.first;
            }
            if (m_game_background.RequestSpaceOnGrid(tetromino_position)) {
                setPosition(tetromino_position);
            }
            break;
        case Direction::left:
            for (auto& square_position : tetromino_position) {
                --square_position.second;
            }
            if (m_game_background.RequestSpaceOnGrid(tetromino_position)) {
                setPosition(tetromino_position);
            }
            break;
        case Direction::right:
            for (auto& square_position : tetromino_position) {
                ++square_position.second;
            }

            if (m_game_background.RequestSpaceOnGrid(tetromino_position)) {
                setPosition(tetromino_position);
            }
            break;
    }
}