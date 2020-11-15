#ifndef GRID_GRAPHIC_H_
#define GRID_GRAPHIC_H_

#include <SFML/Graphics.hpp>
#include <optional>

class GridGraphic : public sf::Drawable {
   public:
    GridGraphic() = default;

    GridGraphic(unsigned int number_rows, unsigned int number_columns,
                float pos_x_top_left_corner, float pos_y_top_left_corner,
                float grid_height);

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