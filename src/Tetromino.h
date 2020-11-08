#ifndef TETROMINO_H_
#define TETROMINO_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "IGridLogic.h"

enum class Direction { left, right, down };

enum class Color {
    cyan,    // (0x00FFFF)
    blue,    // (0x0000FF)
    orange,  // (0xFF8040)
    yellow,  // (0xFFFF00)
    green,   // (0x00FF00)
    purple,  // (0x800080)
    red      // (0xFF0000)
};

enum class TetrominoType { I, O, T, J, L, S, Z };

enum class Orientation { north, east, south, west };

using TetrominoPositionType = std::vector<std::pair<int, int>>;

TetrominoPositionType operator+(const TetrominoPositionType &position1,
                                const TetrominoPositionType &position2);

class Tetromino {
   public:
    Tetromino() = delete;
    Tetromino(IGridLogic &grid_logic, TetrominoPositionType init_position,
              Color color);
    Color GetColor() const { return m_color; };
    TetrominoPositionType GetPosition() const;
    void MoveOneStep(Direction);
    bool IsMoveable() const { return m_is_moveable; };
    void LockMovement() { m_is_moveable = false; };

   protected:
    void SetPosition(TetrominoPositionType);
    TetrominoPositionType m_position;
    IGridLogic &m_grid_logic;

   private:
    Color m_color;
    // m_is_moveable indicates whether the tetromino can be moved or not. When
    // the object is instantiated, the m_is_moveable is true and consequently
    // the tetromino can be moved. Once tetromino has riched the lowest obstacle
    // (be it the lower grid bound or other tetrominos located already at the
    // bottom of the grid) m_is_moveable is switched to false and hence the
    // tetromino can't be moved anymore.
    bool m_is_moveable;
};

class ShapeI : public Tetromino {
   public:
    ShapeI() = delete;
    ShapeI(IGridLogic &grid_logic);
    Orientation GetOrientation();
    void Rotate();

   private:
    const std::map<std::string, TetrominoPositionType> m_delta_positions{
        {"NorthEast", {{-2, 2}, {-1, 1}, {0, 0}, {1, -1}}},
        {"EastSouth", {{2, 1}, {1, 0}, {0, -1}, {-1, -2}}},
        {"SouthWest", {{1, -2}, {0, -1}, {-1, 0}, {-2, 1}}},
        {"WestNorth", {{-1, -1}, {0, 0}, {1, 1}, {2, 2}}}};
    Orientation m_orientation;
};

#endif /* TETROMINO_H_ */