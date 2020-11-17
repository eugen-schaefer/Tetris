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

enum class TetrominoType { I, J, L, O, S, T, Z, UNDEFINED };

enum class Orientation { north, east, south, west };

using TetrominoPositionType = std::vector<std::pair<int, int>>;

using LogicalSquaresIteratorType = TetrominoPositionType::iterator;

TetrominoPositionType operator+(const TetrominoPositionType &position1,
                                const TetrominoPositionType &position2);

class Tetromino {
   public:
    Tetromino() = delete;
    Tetromino(IGridLogic &grid_logic, TetrominoPositionType init_position,
              Color color);
    virtual Color GetColor() const { return m_color; }
    virtual TetrominoPositionType GetPosition() const { return m_position; }
    // This method is supposed to be used only to set tetrominos in the
    // dashboard. Since SetPositionInDashboard() places shapes on the grid
    // regardless of the grid occupancy on the corresponding place,
    // the MoveOneStep() shall be used for game usage instead.
    virtual void SetPositionInDashboard(TetrominoPositionType position) {
        m_position = position;
    }
    virtual bool MoveOneStep(Direction);
    virtual bool IsMovable() const { return m_is_movable; }
    virtual void MakeUnmovable() { m_is_movable = false; };
    virtual void MakeMovable() {
        m_is_movable = true;
    };  // TODO(Eugen): add missing tests
    virtual void Rotate() {}
    virtual void DeleteTetrominoSquare(
        LogicalSquaresIteratorType &logical_iterator);
    virtual LogicalSquaresIteratorType GetIteratorToBeginOfPositionVector() {
        return m_position.begin();
    };
    virtual TetrominoType GetTetrominoType() {
        return TetrominoType::UNDEFINED;
    };

   protected:
    void SetPosition(TetrominoPositionType position) {
        m_position = std::move(position);
    }
    TetrominoPositionType m_position;
    IGridLogic &m_grid_logic;

   private:
    Color m_color;
    // m_is_movable indicates whether the tetromino can be moved or not. When
    // the object is instantiated, the m_is_movable is true and consequently
    // the tetromino can be moved. Once tetromino has riched the lowest obstacle
    // (be it the lower grid bound or other tetrominos located already at the
    // bottom of the grid) m_is_movable is switched to false and hence the
    // tetromino can't be moved anymore.
    bool m_is_movable;
};

class ShapeI : public Tetromino {
   public:
    ShapeI() = delete;
    ShapeI(IGridLogic &grid_logic, TetrominoPositionType init_position = {
                                       {0, 0}, {0, 1}, {0, 2}, {0, 3}});
    Orientation GetOrientation() { return m_orientation; }
    void Rotate() override;
    TetrominoType GetTetrominoType() override { return m_tetromino_type; };

   private:
    const std::map<std::string, TetrominoPositionType> m_delta_positions{
        {"NorthEast", {{-2, 2}, {-1, 1}, {0, 0}, {1, -1}}},
        {"EastSouth", {{2, 1}, {1, 0}, {0, -1}, {-1, -2}}},
        {"SouthWest", {{1, -2}, {0, -1}, {-1, 0}, {-2, 1}}},
        {"WestNorth", {{-1, -1}, {0, 0}, {1, 1}, {2, 2}}}};
    Orientation m_orientation;
    TetrominoType m_tetromino_type;
};

class ShapeJ : public Tetromino {
   public:
    ShapeJ() = delete;
    ShapeJ(IGridLogic &grid_logic, TetrominoPositionType init_position = {
                                       {0, 0}, {1, 0}, {1, 1}, {1, 2}});
    Orientation GetOrientation() { return m_orientation; }
    void Rotate() override;
    TetrominoType GetTetrominoType() override { return m_tetromino_type; };

   private:
    const std::map<std::string, TetrominoPositionType> m_delta_positions{
        {"NorthEast", {{0, 2}, {-1, 1}, {0, 0}, {1, -1}}},
        {"EastSouth", {{2, 0}, {1, 1}, {0, 0}, {-1, -1}}},
        {"SouthWest", {{0, -2}, {1, -1}, {0, 0}, {-1, 1}}},
        {"WestNorth", {{-2, 0}, {-1, -1}, {0, 0}, {1, 1}}}};
    Orientation m_orientation;
    TetrominoType m_tetromino_type;
};

class ShapeL : public Tetromino {
   public:
    ShapeL() = delete;
    ShapeL(IGridLogic &grid_logic, TetrominoPositionType init_position = {
                                       {1, 0}, {1, 1}, {1, 2}, {0, 2}});
    Orientation GetOrientation() { return m_orientation; }
    void Rotate() override;
    TetrominoType GetTetrominoType() override { return m_tetromino_type; };

   private:
    const std::map<std::string, TetrominoPositionType> m_delta_positions{
        {"NorthEast", {{2, 0}, {-1, 1}, {0, 0}, {1, -1}}},
        {"EastSouth", {{0, -2}, {1, 1}, {0, 0}, {-1, -1}}},
        {"SouthWest", {{-2, 0}, {-1, -1}, {0, 0}, {1, 1}}},
        {"WestNorth", {{0, 2}, {1, -1}, {0, 0}, {-1, 1}}}};
    Orientation m_orientation;
    TetrominoType m_tetromino_type;
};

#endif /* TETROMINO_H_ */
