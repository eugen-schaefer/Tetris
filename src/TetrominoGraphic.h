#ifndef TETROMINO_GRAPHIC_H_
#define TETROMINO_GRAPHIC_H_

#include <SFML/Graphics.hpp>
#include <memory>

#include "GridGraphic.h"
#include "Tetromino.h"

using GraphicalSquaresIteratorType = std::vector<sf::RectangleShape>::iterator;

class TetrominoGraphic : public sf::Drawable {
   public:
    TetrominoGraphic(std::unique_ptr<Tetromino> shape,
                     const GridGraphic& grid_graphic);

    bool MoveOneStep(Direction direction);
    void Rotate();
    bool IsMovable() const { return m_shape->IsMovable(); }
    void DeleteTetrominoSquare(GraphicalSquaresIteratorType& graphical_iterator,
                               LogicalSquaresIteratorType& logical_iterator);
    void MakeMovable() { m_shape->MakeMovable(); };
    void MakeUnmovable() { m_shape->MakeUnmovable(); };
    LogicalSquaresIteratorType GetIteratorToBeginOfLogicalTetromino() {
        return m_shape->GetIteratorToBeginOfPositionVector();
    };
    GraphicalSquaresIteratorType GetIteratorToBeginOfGraphicalTetromino() {
        return m_squares.begin();
    };
    GraphicalSquaresIteratorType GetIteratorToEndOfGraphicalTetromino() {
        return m_squares.end();
    };
    std::vector<sf::RectangleShape> GetSquares() const {return m_squares;};

   private:
    std::unique_ptr<Tetromino> m_shape;
    const GridGraphic& m_grid_graphic;
    std::vector<sf::RectangleShape> m_squares;

    void UpdatePosition();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif /* TETROMINO_GRAPHIC_H_ */
