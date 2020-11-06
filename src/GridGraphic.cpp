#include "GridGraphic.h"

GridGraphic::GridGraphic(unsigned int number_vertical_cells,
                         unsigned int number_horizontal_cells,
                         float relative_margin,
                         const sf::Vector2<unsigned int>& window_size) {
    float window_width{static_cast<float>(window_size.x)};
    float window_height{static_cast<float>(window_size.y)};
    m_horizontal_lines =
        sf::VertexArray(sf::Lines, 2 * (number_vertical_cells + 1));
    m_vertical_lines =
        sf::VertexArray(sf::Lines, 2 * (number_horizontal_cells + 1));

    // construct horizontal lines
    float grid_height_in_pixel{(1.0f - 2.0f * relative_margin) * window_height};
    float grid_cell_side_length_in_pixel{
        grid_height_in_pixel / static_cast<float>(number_vertical_cells)};
    float grid_width_in_pixel{static_cast<float>(number_horizontal_cells) *
                              grid_cell_side_length_in_pixel};

    float pos_y_next_horizontal_line{relative_margin * window_height};
    float pos_x_left_gridborder{(1.0f - relative_margin) * window_width -
                                grid_width_in_pixel};
    for (int i{0}; i < 2 * number_vertical_cells + 1; i += 2) {
        // left end of the horizontal line
        m_horizontal_lines[i].position =
            sf::Vector2f(pos_x_left_gridborder, pos_y_next_horizontal_line);
        m_horizontal_lines[i].color = sf::Color::Blue;

        // right end of the horizontal line
        m_horizontal_lines[i + 1].position =
            sf::Vector2f(pos_x_left_gridborder + grid_width_in_pixel,
                         pos_y_next_horizontal_line);
        m_horizontal_lines[i + 1].color = sf::Color::Red;

        pos_y_next_horizontal_line += grid_cell_side_length_in_pixel;
    }

    // construct vertical lines
    float pos_x_next_vertical_line{(1.0f - relative_margin) * window_width -
                                   grid_width_in_pixel};
    float pos_y_top_gridborder{relative_margin * window_height};
    for (int i{0}; i < 2 * number_horizontal_cells + 1; i += 2) {
        // top end of the vertical line
        m_vertical_lines[i].position =
            sf::Vector2f(pos_x_next_vertical_line, pos_y_top_gridborder);
        m_vertical_lines[i].color = sf::Color::Red;

        // bottom end of the vertical line
        m_vertical_lines[i + 1].position =
            sf::Vector2f(pos_x_next_vertical_line,
                         pos_y_top_gridborder + grid_height_in_pixel);
        m_vertical_lines[i + 1].color = sf::Color::Green;

        pos_x_next_vertical_line += grid_cell_side_length_in_pixel;
    }
};

void GridGraphic::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_horizontal_lines, states);
    target.draw(m_vertical_lines, states);
}