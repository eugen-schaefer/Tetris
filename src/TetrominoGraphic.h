#ifndef TETROMINO_GRAPHIC_H_
#define TETROMINO_GRAPHIC_H_

#include <SFML/Graphics.hpp>

#include "GridGraphic.h"
#include "Tetromino.h"

class TetrominoGraphic : public sf::Drawable {
   public:
    TetrominoGraphic(const Tetromino& shape, const GridGraphic& grid_graphic);

    // update position of the drawable tetromino shape
    void UpdatePosition();

   private:
    const Tetromino& m_shape;
    const GridGraphic& m_grid_graphic;
    std::array<sf::RectangleShape, 4u> m_squares;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif /* TETROMINO_GRAPHIC_H_ */