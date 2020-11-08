#ifndef GRID_GRAPHIC_H_
#define GRID_GRAPHIC_H_

#include <SFML/Graphics.hpp>
#include <optional>

class GridGraphic : public sf::Drawable {
   public:
    // First entry in window_size corresponds to the window width and the second
    // one to the window height.
    GridGraphic(unsigned int number_rows, unsigned int number_columns,
                float relative_margin,
                const sf::Vector2<unsigned int>& window_size);

    // row = 0 corresponds to the top row and row = m-1 corresponds to the
    // bottom row.
    // column = 0 corresponds to the most left column and column = n-1
    // corresponds to the most right column.
    std::optional<sf::Vector2f> GetPositionRelativeToWindow(
        int row_index, int column_index) const;

    float GetGridCellSideLength() const { return m_grid_cell_side_length; };

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray m_horizontal_lines;
    sf::VertexArray m_vertical_lines;
    float m_grid_cell_side_length;
};

#endif /* GRID_GRAPHIC_H_ */