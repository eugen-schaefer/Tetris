#ifndef GRID_GRAPHIC_H_
#define GRID_GRAPHIC_H_

#include <SFML/Graphics.hpp>

class GridGraphic : public sf::Drawable {
   public:
    GridGraphic(unsigned int number_vertical_cells,
                unsigned int number_horizontal_cells, float relative_margin,
                const sf::Vector2<unsigned int>& window_size);

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray m_horizontal_lines;
    sf::VertexArray m_vertical_lines;
};

#endif /* GRID_GRAPHIC_H_ */