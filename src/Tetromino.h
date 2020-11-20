#ifndef TETROMINO_H_
#define TETROMINO_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "IGridLogic.h"

enum class Direction { left, right, down };

enum class Color {
    cyan,     // (0x00FFFF)
    blue,     // (0x0000FF)
    orange,   // (0xFF8040)
    yellow,   // (0xFFFF00)
    green,    // (0x00FF00)
    magenta,  // (0xFF00FF)
    red       // (0xFF0000)
};

enum class TetrominoType { I, J, L, O, S, T, Z, UNDEFINED };

enum class Orientation { north, east, south, west };

using TetrominoPositionType = std::vector<std::pair<int, int>>;

using LogicalSquaresIteratorType = TetrominoPositionType::iterator;

TetrominoPositionType operator+(const TetrominoPositionType &position1,
                                const TetrominoPositionType &position2);

/// Tetromino class is the base class for all seven tetromino shapes. It
/// provides common properties like color and actions like a move in a certain
/// direction for all shapes.
class Tetromino {
   public:
    /// No default constructor provided to enforce creating shapes with a fixed
    /// number of squares.
    Tetromino() = delete;

    /// Creates a tetromino consisting of exactly four squares clearly
    /// determined by their positions in the grid.
    /// \param grid_logic:    a reference to the logical grid to determine
    ///                       tetrominoes' unambiguous position in the grid.
    /// \param init_position: unambiguous position in the grid where the created
    ///                       tetromino is initially placed.
    Tetromino(IGridLogic &grid_logic, TetrominoPositionType init_position,
              Color color);

    /// Retrieves tetrominoes' color.
    /// \return tetrominoes' color
    virtual Color GetColor() const { return m_color; }

    /// Retrieves tetrominoes' position.
    /// \return position of every tetromino square
    virtual TetrominoPositionType GetPosition() const { return m_position; }

    /// SetPositionInDashboard is supposed to be used only to set the tetromino
    /// in the dashboard. Since SetPosition() places shapes on the grid
    /// regardless of the grid occupancy at the corresponding place, the
    /// MoveOneStep() shall be used for game usage instead.
    /// \param target_position: Position in the logical grid at which the
    ///                         Tetromino is to be placed.
    virtual void SetPosition(TetrominoPositionType target_position) {
        m_position = std::move(target_position);
    }

    /// Moves the tetromino one step towards a specified direction
    /// \param direction: direction towards which the movement shall take place.
    /// \return returns true when the movement was successful, false otherwise.
    virtual bool MoveOneStep(Direction direction);

    /// Determines whether the tetronimo is locked down or not
    /// \return true if tetromino is locked down and hence unmovable, false
    /// otherwise
    virtual bool IsLocked() const { return m_is_locked; }

    /// Lock tetromino such that it cant be moved or rotated.
    virtual void LockDown() { m_is_locked = true; };

    /// Unlock tetromino such that it can be moved or rotated.
    virtual void Release() { m_is_locked = false; };

    virtual void Rotate() {}

    /// Deletes one specified square within the tetromino initially consisting
    /// of four squares. This function is supposed to be used in line clearing
    /// cases.
    /// \param logical_iterator: iterator to the square being deleted
    virtual void DeleteTetrominoSquare(
        LogicalSquaresIteratorType &logical_iterator);

    /// Retrieved the iterator to tetrominoes first square.
    virtual LogicalSquaresIteratorType GetIteratorToBeginOfPositionVector() {
        return m_position.begin();
    };

    /// Retrieves the information about the concrete shape. Since the base class
    /// is not meant to represent a concrete shape, its type is undefined.
    virtual TetrominoType GetTetrominoType() {
        return TetrominoType::UNDEFINED;
    };

   protected:
    TetrominoPositionType m_position;
    IGridLogic &m_grid_logic;

   private:
    Color m_color;

    // m_is_locked indicates whether the tetromino can be moved or not. When
    // the object is instantiated, the m_is_locked is false and consequently
    // the tetromino can be moved. Once tetromino has riched the lowest obstacle
    // (be it the lower grid bound or other tetrominos located already at the
    // bottom of the grid) m_is_locked is switched to true and hence the
    // tetromino can't be moved anymore because it's locked down.
    bool m_is_locked;
};

/// The ShapeI class represents concrete shape I. Apart from inherited methods,
/// it provides in addition a concrete implementation of the shape rotation.
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

/// The ShapeJ class represents concrete shape J. Apart from inherited methods,
/// it provides in addition a concrete implementation of the shape rotation.
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

/// The ShapeL class represents concrete shape L. Apart from inherited methods,
/// it provides in addition a concrete implementation of the shape rotation.
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

/// The ShapeO class represents concrete shape O. Apart from inherited methods,
/// it provides in addition a concrete implementation of the shape rotation.
class ShapeO : public Tetromino {
   public:
    ShapeO() = delete;
    ShapeO(IGridLogic &grid_logic, TetrominoPositionType init_position = {
                                       {0, 0}, {0, 1}, {1, 1}, {1, 0}});
    Orientation GetOrientation() { return m_orientation; }
    void Rotate() override;
    TetrominoType GetTetrominoType() override { return m_tetromino_type; };

   private:
    Orientation m_orientation;
    TetrominoType m_tetromino_type;
};

/// The ShapeS class represents concrete shape S. Apart from inherited methods,
/// it provides in addition a concrete implementation of the shape rotation.
class ShapeS : public Tetromino {
   public:
    ShapeS() = delete;
    ShapeS(IGridLogic &grid_logic, TetrominoPositionType init_position = {
                                       {0, 1}, {0, 2}, {1, 1}, {1, 0}});
    Orientation GetOrientation() { return m_orientation; }
    void Rotate() override;
    TetrominoType GetTetrominoType() override { return m_tetromino_type; };

   private:
    const std::map<std::string, TetrominoPositionType> m_delta_positions{
        {"NorthEast", {{1, 1}, {2, 0}, {0, 0}, {-1, 1}}},
        {"EastSouth", {{1, -1}, {0, -2}, {0, 0}, {1, 1}}},
        {"SouthWest", {{-1, -1}, {-2, 0}, {0, 0}, {1, -1}}},
        {"WestNorth", {{-1, 1}, {0, 2}, {0, 0}, {-1, -1}}}};
    Orientation m_orientation;
    TetrominoType m_tetromino_type;
};

/// The ShapeT class represents concrete shape T. Apart from inherited methods,
/// it provides in addition a concrete implementation of the shape rotation.
class ShapeT : public Tetromino {
   public:
    ShapeT() = delete;
    ShapeT(IGridLogic &grid_logic, TetrominoPositionType init_position = {
                                       {0, 1}, {1, 0}, {1, 1}, {1, 2}});
    Orientation GetOrientation() { return m_orientation; }
    void Rotate() override;
    TetrominoType GetTetrominoType() override { return m_tetromino_type; };

   private:
    const std::map<std::string, TetrominoPositionType> m_delta_positions{
        {"NorthEast", {{0, 0}, {0, 2}, {0, 0}, {1, -1}}},
        {"EastSouth", {{2, 0}, {0, 0}, {0, 0}, {-1, -1}}},
        {"SouthWest", {{-1, -1}, {1, -1}, {0, 0}, {-1, 1}}},
        {"WestNorth", {{-1, 1}, {-1, -1}, {0, 0}, {1, 1}}}};
    Orientation m_orientation;
    TetrominoType m_tetromino_type;
};

/// The ShapeZ class represents concrete shape Z. Apart from inherited methods,
/// it provides in addition a concrete implementation of the shape rotation.
class ShapeZ : public Tetromino {
   public:
    ShapeZ() = delete;
    ShapeZ(IGridLogic &grid_logic, TetrominoPositionType init_position = {
                                       {0, 0}, {0, 1}, {1, 1}, {1, 2}});
    Orientation GetOrientation() { return m_orientation; }
    void Rotate() override;
    TetrominoType GetTetrominoType() override { return m_tetromino_type; };

   private:
    const std::map<std::string, TetrominoPositionType> m_delta_positions{
        {"NorthEast", {{0, 2}, {1, 1}, {0, 0}, {1, -1}}},
        {"EastSouth", {{2, 0}, {1, -1}, {0, 0}, {-1, -1}}},
        {"SouthWest", {{0, -2}, {-1, -1}, {0, 0}, {-1, 1}}},
        {"WestNorth", {{-2, 0}, {-1, 1}, {0, 0}, {1, 1}}}};
    Orientation m_orientation;
    TetrominoType m_tetromino_type;
};

#endif /* TETROMINO_H_ */
