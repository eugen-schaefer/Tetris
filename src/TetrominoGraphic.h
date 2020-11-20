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
    bool IsLocked() const { return m_shape->IsLocked(); }
    void DeleteTetrominoSquare(GraphicalSquaresIteratorType& graphical_iterator,
                               LogicalSquaresIteratorType& logical_iterator);
    void LockDown() { m_shape->LockDown(); };
    void Release() { m_shape->Release(); };
    LogicalSquaresIteratorType GetIteratorToBeginOfLogicalTetromino() {
        return m_shape->GetIteratorToBeginOfPositionVector();
    };
    GraphicalSquaresIteratorType GetIteratorToBeginOfGraphicalTetromino() {
        return m_squares.begin();
    };
    GraphicalSquaresIteratorType GetIteratorToEndOfGraphicalTetromino() {
        return m_squares.end();
    };
    std::vector<sf::RectangleShape> GetSquares() const { return m_squares; };

    TetrominoType GetTetrominoType() { return m_shape->GetTetrominoType(); };

    void SetPositionInDashboard(TetrominoPositionType position);

    TetrominoPositionType GetPositionInGridLogicFrame() {
        return m_shape->GetPosition();
    }

    int GetHighestRow();

   private:
    std::unique_ptr<Tetromino> m_shape;
    const GridGraphic& m_grid_graphic;
    std::vector<sf::RectangleShape> m_squares;

    void UpdatePosition();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif /* TETROMINO_GRAPHIC_H_ */
