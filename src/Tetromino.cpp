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
    TetrominoPositionType position = getPosition();
    switch (direction) {
        case Direction::down:
            for (auto& pos : position) {  // TODO: come up with besser naming
                                          // for position/pos
                ++pos.first;
            }
            if (m_game_background.RequestSpaceOnGrid(position)) {
                setPosition(position);
            }
            break;
        case Direction::left:
            for (auto& pos : position) {  // TODO: come up with besser naming
                                          // for position/pos
                --pos.second;
            }
            if (m_game_background.RequestSpaceOnGrid(position)) {
                setPosition(position);
            }
            break;
        case Direction::right:
            for (auto& pos : position) {  // TODO: come up with besser naming
                                          // for position/pos
                ++pos.second;
            }
            // bool my_var = m_game_background.RequestSpaceOnGrid(position);
            if (m_game_background.RequestSpaceOnGrid(position)) {
                setPosition(position);
            }
            break;

            //        default:
            //           break;
    }
}