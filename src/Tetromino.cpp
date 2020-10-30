#include "Tetromino.h"

TetrominoPositionType operator+(const TetrominoPositionType &position1,
                                const TetrominoPositionType &position2) {
    // TODO: static_assert(position1.size() == position2.size(), "The size of
    // the lhs and rhs of the + operator must match!");

    TetrominoPositionType result{{0, 0}, {0, 0}, {0, 0}, {0, 0}};
    for (int idx{0}; idx < result.size(); ++idx) {
        result.at(idx).first =
            position1.at(idx).first + position2.at(idx).first;
        result.at(idx).second =
            position1.at(idx).second + position2.at(idx).second;
    }
    return result;
}

Tetromino::Tetromino(IGameBackground &game_background,
                     TetrominoPositionType init_position, Color color)
    : m_game_background{game_background},
      m_position{init_position},
      m_color{color} {};

Color Tetromino::GetColor() { return m_color; }

TetrominoPositionType Tetromino::GetPosition() { return m_position; }

void Tetromino::SetPosition(TetrominoPositionType position) {
    m_position = position;
}

void Tetromino::MoveOneStep(Direction direction) {
    TetrominoPositionType tetromino_position = GetPosition();
    switch (direction) {
        case Direction::down:
            for (auto &square_position : tetromino_position) {
                ++square_position.first;
            }
            break;
        case Direction::left:
            for (auto &square_position : tetromino_position) {
                --square_position.second;
            }
            break;
        case Direction::right:
            for (auto &square_position : tetromino_position) {
                ++square_position.second;
            }
            break;
    }
    if (m_game_background.RequestSpaceOnGrid(tetromino_position)) {
        SetPosition(tetromino_position);
    }
}

ShapeI::ShapeI(IGameBackground &game_background,
               TetrominoPositionType init_position, Color color)
    : Tetromino{game_background, init_position, color},
      m_orientation{Orientation::north} {}

Orientation ShapeI::GetOrientation() { return m_orientation; }

void ShapeI::Rotate() {
    TetrominoPositionType position_after_rotation{};
    switch (m_orientation) {
        case Orientation::north:
            position_after_rotation =
                GetPosition() + m_delta_positions.at("NorthEast");
            if (m_game_background.RequestSpaceOnGrid(position_after_rotation)) {
                SetPosition(position_after_rotation);
                m_orientation = Orientation::east;
            }
            break;
        case Orientation::east:
            position_after_rotation =
                GetPosition() + m_delta_positions.at("EastSouth");
            if (m_game_background.RequestSpaceOnGrid(position_after_rotation)) {
                SetPosition(position_after_rotation);
                m_orientation = Orientation::east;
            }
            break;
        case Orientation::south:
            position_after_rotation =
                GetPosition() + m_delta_positions.at("SouthWest");
            if (m_game_background.RequestSpaceOnGrid(position_after_rotation)) {
                SetPosition(position_after_rotation);
                m_orientation = Orientation::east;
            }
            break;
        case Orientation::west:
            position_after_rotation =
                GetPosition() + m_delta_positions.at("WestNorth");
            if (m_game_background.RequestSpaceOnGrid(position_after_rotation)) {
                SetPosition(position_after_rotation);
                m_orientation = Orientation::east;
            }
            break;
    }

    if (m_game_background.RequestSpaceOnGrid(position_after_rotation)) {
        SetPosition(position_after_rotation);
    }
}