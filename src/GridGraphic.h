#ifndef GRID_GRAPHIC_H_
#define GRID_GRAPHIC_H_

#include <SFML/Graphics.hpp>
#include <optional>

/// The GridGraphic class draws the grid on the screen into which the single
/// tetrominoes are supposed to be drawn. As opposed to the GridLogic, the
/// x-axis is horizontal and counts positive from left to right whereas the
/// y-axis is vertical counting positive from top to bottom.
class GridGraphic : public sf::Drawable {
   public:
    GridGraphic() = default;

    /// Constructs horizontal and vertical lines with certain properties like
    /// length, color, a position which finally are drawn on the screen to shape
    /// a grid.
    /// \param number_rows:     number of rows in the grid being constructed
    /// \param number_columns:  number of columns in the grid eing constructed
    /// \param pos_x_top_left_corner:   horizontal distance in pixel between the
    ///                                 game window's left edge and the grid's
    ///                                 left edge.
    /// \param pos_y_top_left_corner:   vertical distance in pixel between the
    ///                                 game window's top edge and the grid's
    ///                                 top edge.
    /// \param grid_height:             the height of the grid in pixel.
    GridGraphic(unsigned int number_rows, unsigned int number_columns,
                float pos_x_top_left_corner, float pos_y_top_left_corner,
                float grid_height);

    /// Retrieves logical grid cell's position in pixel relative to the game
    /// window. The reference point of the cell's position is its left upper
    /// corner point.
    /// \param row_index:       row_index = 0 corresponds to the top row and
    ///                         row = m-1 corresponds to the bottom row.
    /// \param column_index:    column_index = 0 corresponds to the most
    ///                         left column and column = n-1 corresponds to
    ///                         the most right column.
    /// \return: x/y coordinates in pixel relative to the game window.
    std::optional<sf::Vector2f> GetPositionRelativeToWindow(
        int row_index, int column_index) const;

    /// Retrieves grid cell's side length in pixel relative to the game window.
    float GetGridCellSideLength() const { return m_grid_cell_side_length; };

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    // contains the endpoints of all horizontal lines
    sf::VertexArray m_horizontal_lines;
    // contains the endpoints of all vertical lines
    sf::VertexArray m_vertical_lines;
    float m_grid_cell_side_length;
};

#endif /* GRID_GRAPHIC_H_ */