#include "../src/GridLogic.h"
#include "gtest/gtest.h"

class GridLogicTest : public ::testing::Test {
   protected:
    GridLogicTest() : grid_logic(vertical_grid_size, horizontal_grid_size) {}
    int horizontal_grid_size{5};
    int vertical_grid_size{10};
    GridLogic grid_logic;
};

TEST_F(GridLogicTest, EmptyGridAtInstantiation) {
    // check whether all grid cells are false
    bool is_occupancy_grid_empty{true};
    auto occupancy_grid = grid_logic.GetOccupancyGrid();
    for (const auto& row : occupancy_grid) {
        for (const bool& is_cell_occupied : row) {
            if (is_cell_occupied) {
                is_occupancy_grid_empty = false;
                break;
            }
        }
    }
    EXPECT_TRUE(is_occupancy_grid_empty);
}

TEST_F(GridLogicTest, ZeroFilledButtomLinesAtInstantiation) {
    int expected_nr_buttomlines_filled{};
    EXPECT_EQ(expected_nr_buttomlines_filled,
              grid_logic.GetNumberFilledBottomLines());
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestForTopLeftCorner) {
    TetrominoPositionType quadrupel{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    bool actual_result = grid_logic.RequestSpaceOnGrid(quadrupel);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestForBottomRightCorner) {
    int m = vertical_grid_size;
    int n = horizontal_grid_size;
    TetrominoPositionType quadrupel{
        {m - 2, n - 2}, {m - 1, n - 2}, {m - 1, n - 1}, {m - 2, n - 1}};
    bool actual_result = grid_logic.RequestSpaceOnGrid(quadrupel);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfLeftBounce) {
    int m = vertical_grid_size;
    int n = horizontal_grid_size;
    TetrominoPositionType quadrupel{{0, -1}, {1, -1}, {1, 0}, {0, 0}};
    bool actual_result = grid_logic.RequestSpaceOnGrid(quadrupel);
    EXPECT_FALSE(actual_result);
}

TEST_F(GridLogicTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfRightBounce) {
    int m = vertical_grid_size;
    int n = horizontal_grid_size;
    TetrominoPositionType quadrupel{{0, horizontal_grid_size - 1},
                                    {1, horizontal_grid_size - 1},
                                    {1, horizontal_grid_size},
                                    {0, horizontal_grid_size}};
    bool actual_result = grid_logic.RequestSpaceOnGrid(quadrupel);
    EXPECT_FALSE(actual_result);
}

TEST_F(GridLogicTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfBottomBounce) {
    int m = vertical_grid_size;
    int n = horizontal_grid_size;
    TetrominoPositionType quadrupel{{vertical_grid_size - 1, 0},
                                    {vertical_grid_size, 0},
                                    {vertical_grid_size, 1},
                                    {vertical_grid_size - 1, 1}};
    bool actual_result = grid_logic.RequestSpaceOnGrid(quadrupel);
    EXPECT_FALSE(actual_result);
}

TEST_F(GridLogicTest, UnsuccessfullGridOccupancyRequestBecauseAlreadyOccupied) {
    TetrominoPositionType quadrupel{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    bool actual_result = grid_logic.RequestSpaceOnGrid(quadrupel);
    EXPECT_TRUE(actual_result);
    actual_result = grid_logic.RequestSpaceOnGrid(quadrupel);
    EXPECT_FALSE(actual_result);
}