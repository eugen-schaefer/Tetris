#include "GridGraphic.h"

// TODO(Eugen): organize the data types of var regarding float/uint because of
// pixel units

GridGraphic::GridGraphic(unsigned int number_rows, unsigned int number_columns,
                         float pos_x_top_left_corner,
                         float pos_y_top_left_corner, float grid_height) {
    m_horizontal_lines = sf::VertexArray(sf::Lines, 2 * (number_rows + 1));
    m_vertical_lines = sf::VertexArray(sf::Lines, 2 * (number_columns + 1));

    // construct horizontal lines
    m_grid_cell_side_length = grid_height / static_cast<float>(number_rows);
    float grid_width{static_cast<float>(number_columns) *
                     m_grid_cell_side_length};
    float pos_x_left_gridborder{pos_x_top_left_corner};
    float pos_y_next_horizontal_line{pos_y_top_left_corner};

    for (int i{0}; i < 2 * number_rows + 1; i += 2) {
        // left end of the horizontal line
        m_horizontal_lines[i].position =
            sf::Vector2f(pos_x_left_gridborder, pos_y_next_horizontal_line);
        m_horizontal_lines[i].color = sf::Color::Blue;

        // right end of the horizontal line
        m_horizontal_lines[i + 1].position = sf::Vector2f(
            pos_x_left_gridborder + grid_width, pos_y_next_horizontal_line);
        m_horizontal_lines[i + 1].color = sf::Color::Red;

        pos_y_next_horizontal_line += m_grid_cell_side_length;
    }

    // construct vertical lines
    float pos_x_next_vertical_line{pos_x_top_left_corner};
    float pos_y_top_gridborder{pos_y_top_left_corner};
    for (int i{0}; i < 2 * number_columns + 1; i += 2) {
        // top end of the vertical line
        m_vertical_lines[i].position =
            sf::Vector2f(pos_x_next_vertical_line, pos_y_top_gridborder);
        m_vertical_lines[i].color = sf::Color::Red;

        // bottom end of the vertical line
        m_vertical_lines[i + 1].position = sf::Vector2f(
            pos_x_next_vertical_line, pos_y_top_gridborder + grid_height);
        m_vertical_lines[i + 1].color = sf::Color::Green;

        pos_x_next_vertical_line += m_grid_cell_side_length;
    }
}

std::optional<sf::Vector2f> GridGraphic::GetPositionRelativeToWindow(
    int row_index, int column_index) const {
    // out of bounds check for passed arguments
    unsigned long total_number_rows =
        (m_horizontal_lines.getVertexCount()) / 2 - 1;
    unsigned long total_number_columns =
        (m_vertical_lines.getVertexCount()) / 2 - 1;
    if (row_index < 0 || row_index > (total_number_rows - 1) ||
        column_index < 0 || column_index > (total_number_columns - 1)) {
        return {};
    }

    // calculate position in case correct arguments are passed
    int horizontal_lines_index = 2 * row_index;
    int vertical_line_index = 2 * column_index;
    float x_position_in_window_frame =
        m_vertical_lines[vertical_line_index].position.x;

    float y_position_in_window_frame =
        m_horizontal_lines[horizontal_lines_index].position.y;
    return std::optional<sf::Vector2f>(
        {x_position_in_window_frame, y_position_in_window_frame});
}

void GridGraphic::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_horizontal_lines, states);
    target.draw(m_vertical_lines, states);
}
