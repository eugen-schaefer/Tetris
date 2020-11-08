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

Tetromino::Tetromino(IGridLogic &grid_logic,
                     TetrominoPositionType init_position, Color color)
    : m_grid_logic{grid_logic},
      m_position{init_position},
      m_color{color},
      m_is_moveable{true} {};

TetrominoPositionType Tetromino::GetPosition() const { return m_position; }

void Tetromino::SetPosition(TetrominoPositionType position) {
    m_position = position;
}

void Tetromino::MoveOneStep(Direction direction) {
    if (!IsMoveable()) {
        return;
    }

    TetrominoPositionType current_position{GetPosition()};
    TetrominoPositionType target_position{current_position};
    switch (direction) {
        case Direction::down:
            for (auto &new_square_position : target_position) {
                ++new_square_position.first;
            }
            break;
        case Direction::left:
            for (auto &new_square_position : target_position) {
                --new_square_position.second;
            }
            break;
        case Direction::right:
            for (auto &new_square_position : target_position) {
                ++new_square_position.second;
            }
            break;
    }
    if (m_grid_logic.RequestSpaceOnGrid(current_position, target_position)) {
        SetPosition(target_position);
    } else if (direction == Direction::down) {
        // TODO(Eugen): Consider waiting a short period of time (e.g. 1 second)
        // before locking. This waiting time would allow to horizontally move
        // the shape at the lowest possible level for the specified amount of
        // time before is is frozen forever.
        LockMovement();
    }
}

// Create I-shape such that it has its initial position in the upper left
// corner, its orientation is horizontal and its color is cyan.
ShapeI::ShapeI(IGridLogic &grid_logic)
    : m_orientation{Orientation::north},
      Tetromino{grid_logic, {{0, 0}, {0, 1}, {0, 2}, {0, 3}}, Color::cyan} {}

Orientation ShapeI::GetOrientation() { return m_orientation; }

void ShapeI::Rotate() {
    if (!IsMoveable()) {
        return;
    }

    TetrominoPositionType current_position{GetPosition()};
    TetrominoPositionType target_position{};
    switch (m_orientation) {
        case Orientation::north:
            target_position =
                current_position + m_delta_positions.at("NorthEast");
            if (m_grid_logic.RequestSpaceOnGrid(current_position,
                                                target_position)) {
                SetPosition(target_position);
                m_orientation = Orientation::east;
            }
            break;
        case Orientation::east:
            target_position =
                current_position + m_delta_positions.at("EastSouth");
            if (m_grid_logic.RequestSpaceOnGrid(current_position,
                                                target_position)) {
                SetPosition(target_position);
                m_orientation = Orientation::south;
            }
            break;
        case Orientation::south:
            target_position =
                current_position + m_delta_positions.at("SouthWest");
            if (m_grid_logic.RequestSpaceOnGrid(current_position,
                                                target_position)) {
                SetPosition(target_position);
                m_orientation = Orientation::west;
            }
            break;
        case Orientation::west:
            target_position =
                current_position + m_delta_positions.at("WestNorth");
            if (m_grid_logic.RequestSpaceOnGrid(current_position,
                                                target_position)) {
                SetPosition(target_position);
                m_orientation = Orientation::north;
            }
            break;
    }
}