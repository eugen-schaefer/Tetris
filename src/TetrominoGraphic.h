#ifndef TETROMINO_GRAPHIC_H_
#define TETROMINO_GRAPHIC_H_

#include <SFML/Graphics.hpp>
#include <memory>

#include "GridGraphic.h"
#include "Tetromino.h"

class TetrominoGraphic : public sf::Drawable {
   public:
    TetrominoGraphic(std::unique_ptr<Tetromino> shape,
                     const GridGraphic& grid_graphic);

    void MoveOneStep(Direction direction);
    void Rotate();
    bool IsMovable() const { return m_shape->IsMovable(); }

   private:
    std::unique_ptr<Tetromino> m_shape;
    const GridGraphic& m_grid_graphic;
    std::array<sf::RectangleShape, 4u> m_squares;

    void UpdatePosition();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif /* TETROMINO_GRAPHIC_H_ */