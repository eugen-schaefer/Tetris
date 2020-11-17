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
      m_is_movable{true} {};

bool Tetromino::MoveOneStep(Direction direction) {
    if (!IsMovable()) {
        return false;
    }

    bool is_movement_succeed{};
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
        is_movement_succeed = true;
    } else if (direction == Direction::down) {
        // TODO(Eugen): Consider waiting a short period of time (e.g. 1 second)
        // before locking. This waiting time would allow to horizontally move
        // the shape at the lowest possible level for the specified amount of
        // time before is is frozen forever.
        MakeUnmovable();
        is_movement_succeed = false;
    }
    return is_movement_succeed;
}

// iterator: Before erasing, it's iterator pointing to the element beeing
//           removed
//           After erasing, it's an iterator following the last removed
//           element
void Tetromino::DeleteTetrominoSquare(LogicalSquaresIteratorType &iterator) {
    iterator = m_position.erase(iterator);
}

// Create I-shape such that it has its initial position in the upper left
// corner, its orientation is horizontal and its color is cyan.
ShapeI::ShapeI(IGridLogic &grid_logic, TetrominoPositionType init_position)
    : m_orientation{Orientation::north},
      Tetromino{grid_logic, init_position, Color::cyan},
      m_tetromino_type{TetrominoType::I} {}

void ShapeI::Rotate() {
    if (!IsMovable()) {
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

// Create J-shape such that it has its initial position in the upper left
// corner, its orientation is horizontal and its color is blue.
ShapeJ::ShapeJ(IGridLogic &grid_logic, TetrominoPositionType init_position)
    : m_orientation{Orientation::north},
      Tetromino{grid_logic, init_position, Color::blue},
      m_tetromino_type{TetrominoType::J} {}

void ShapeJ::Rotate() {
    if (!IsMovable()) {
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

// Create L-shape such that it has its initial position in the upper left
// corner, its orientation is horizontal and its color is orange.
ShapeL::ShapeL(IGridLogic &grid_logic, TetrominoPositionType init_position)
    : m_orientation{Orientation::north},
      Tetromino{grid_logic, init_position, Color::orange},
      m_tetromino_type{TetrominoType::L} {}

void ShapeL::Rotate() {
    if (!IsMovable()) {
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

// Create O-shape such that it has its initial position in the upper left
// corner, its orientation is horizontal and its color is yellow.
ShapeO::ShapeO(IGridLogic &grid_logic, TetrominoPositionType init_position)
    : m_orientation{Orientation::north},
      Tetromino{grid_logic, init_position, Color::yellow},
      m_tetromino_type{TetrominoType::O} {}

void ShapeO::Rotate() {}
