#ifndef TETROMINO_GRAPHIC_H_
#define TETROMINO_GRAPHIC_H_

#include <SFML/Graphics.hpp>
#include <memory>

#include "GridGraphic.h"
#include "Tetromino.h"

using GraphicalSquaresIteratorType = std::vector<sf::RectangleShape>::iterator;

/// TetrominoGraphic class draws a concrete tetromino shape on the screen. Its
/// size and position is relative to the associated grid in which the shape is
/// beeing drawn.
class TetrominoGraphic : public sf::Drawable {
   public:
    /// Constructs a concrete tetromino shape out of four adjacent squares as a
    /// graphical object.
    /// \param shape: tetromino shape being drawn
    /// \param grid_graphic: grid in which tetromino is being drawn
    TetrominoGraphic(std::unique_ptr<Tetromino> shape,
                     const GridGraphic& grid_graphic);

    /// wraps the same-named method from the tetromino class to then update the
    /// actual drawing. \param direction: direction towards which the movement
    /// shall take place. \return returns true when the movement was successful,
    /// false otherwise.
    bool MoveOneStep(Direction direction);

    /// Rotates the tetromino clockwise
    void Rotate();

    /// wraps the same-named method from the tetromino class.
    /// \return true if tetromino is locked down and hence unmovable, false
    /// otherwise
    bool IsLocked() const { return m_shape->IsLocked(); }

    /// wraps the same-named method from the tetromino class to then update the
    /// actual drawing. \param logical_iterator: iterator to the square being
    /// deleted
    void DeleteTetrominoSquare(GraphicalSquaresIteratorType& graphical_iterator,
                               LogicalSquaresIteratorType& logical_iterator);

    /// wraps the same-named method from the tetromino class.
    void LockDown() { m_shape->LockDown(); };

    /// wraps the same-named method from the tetromino class.
    void Release() { m_shape->Release(); };

    /// Retrieves the iterator to the first square of the Tetromino class object
    LogicalSquaresIteratorType GetIteratorToBeginOfLogicalTetromino() {
        return m_shape->GetIteratorToBeginOfPositionVector();
    };

    /// Retrieves the iterator to the first square of the TetrominoGraphic class
    /// object
    GraphicalSquaresIteratorType GetIteratorToBeginOfGraphicalTetromino() {
        return m_squares.begin();
    };

    /// Retrieves the iterator to the last square of the TetrominoGraphic class
    /// object
    GraphicalSquaresIteratorType GetIteratorToEndOfGraphicalTetromino() {
        return m_squares.end();
    };

    /// Retrieves all squares of the TetrominoGraphic class object
    std::vector<sf::RectangleShape> GetSquares() const { return m_squares; };

    /// wraps the same-named method from the tetromino class.
    TetrominoType GetTetrominoType() { return m_shape->GetTetrominoType(); };

    /// This function shall only be used to draw tetrominoes on the dashboard.
    void SetPositionInDashboard(TetrominoPositionType position);

    /// Retrieves the tetrominoes position relative to logical grid coordinates.
    TetrominoPositionType GetPositionInGridLogicFrame() {
        return m_shape->GetPosition();
    }

    /// Retrieves the index of the row in which the highest positioned tetromino
    /// square resides.
    int GetHighestRow();

   private:
    std::unique_ptr<Tetromino> m_shape;
    const GridGraphic& m_grid_graphic;
    std::vector<sf::RectangleShape> m_squares;

    void UpdatePosition();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif /* TETROMINO_GRAPHIC_H_ */
