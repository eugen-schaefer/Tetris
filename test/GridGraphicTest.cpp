#include <SFML/Graphics.hpp>

#include "../src/GridGraphic.h"
#include "gtest/gtest.h"

class GridGraphicTest : public ::testing::Test {
   protected:
    GridGraphicTest()
        : grid_graphic{number_rows, number_columns, pos_x_top_left_corner,
                       pos_y_top_left_corner, grid_height} {};
    unsigned int number_rows{4};
    unsigned int number_columns{3};
    float pos_x_top_left_corner{200.f};
    float pos_y_top_left_corner{30.f};
    float grid_height{600};

    GridGraphic grid_graphic;
};

TEST_F(GridGraphicTest, test_positions_of_two_horizontal_cells) {
    int left_cell_row_index{0};
    int left_cell_column_index{0};
    int right_cell_row_index{0};
    int right_cell_column_index{1};

    sf::Vector2f left_cell_position_in_window_frame =
        *grid_graphic.GetPositionRelativeToWindow(left_cell_row_index,
                                                  left_cell_column_index);

    sf::Vector2f right_cell_position_in_window_frame =
        *grid_graphic.GetPositionRelativeToWindow(right_cell_row_index,
                                                  right_cell_column_index);

    EXPECT_LT(left_cell_position_in_window_frame.x,
              right_cell_position_in_window_frame.x);

    EXPECT_FLOAT_EQ(left_cell_position_in_window_frame.y,
                    right_cell_position_in_window_frame.y);
}

TEST_F(GridGraphicTest, test_positions_of_two_vertical_cells) {
    int upper_cell_row_index{0};
    int upper_cell_column_index{0};
    int lower_cell_row_index{1};
    int lower_cell_column_index{0};

    sf::Vector2f upper_cell_position_in_window_frame =
        *grid_graphic.GetPositionRelativeToWindow(upper_cell_row_index,
                                                  upper_cell_column_index);

    sf::Vector2f lower_cell_position_in_window_frame =
        *grid_graphic.GetPositionRelativeToWindow(lower_cell_row_index,
                                                  lower_cell_column_index);

    EXPECT_FLOAT_EQ(upper_cell_position_in_window_frame.x,
                    lower_cell_position_in_window_frame.x);

    EXPECT_LT(upper_cell_position_in_window_frame.y,
              lower_cell_position_in_window_frame.y);
}

TEST_F(GridGraphicTest,
       test_positions_of_two_vertical_cells_passing_negative_row_index) {
    int row_index{-1};
    int column_index{0};

    auto position_in_window_frame =
        grid_graphic.GetPositionRelativeToWindow(row_index, column_index);

    EXPECT_FALSE(position_in_window_frame);
}

TEST_F(GridGraphicTest,
       test_positions_of_two_vertical_cells_passing_negative_column_index) {
    int row_index{0};
    int column_index{-1};

    auto position_in_window_frame =
        grid_graphic.GetPositionRelativeToWindow(row_index, column_index);

    EXPECT_FALSE(position_in_window_frame);
}

TEST_F(GridGraphicTest,
       test_positions_of_two_vertical_cells_passing_too_big_row_index) {
    int row_index{static_cast<int>(number_rows)};
    int column_index{0};

    auto position_in_window_frame =
        grid_graphic.GetPositionRelativeToWindow(row_index, column_index);

    EXPECT_FALSE(position_in_window_frame);
}

TEST_F(GridGraphicTest,
       test_positions_of_two_vertical_cells_passing_too_big_column_index) {
    int row_index{0};
    int column_index{static_cast<int>(number_columns)};

    auto position_in_window_frame =
        grid_graphic.GetPositionRelativeToWindow(row_index, column_index);

    EXPECT_FALSE(position_in_window_frame);
}