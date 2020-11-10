#include "../src/GridLogic.h"
#include "gtest/gtest.h"

class GridLogicTest : public ::testing::Test {
   protected:
    GridLogicTest() : grid_logic(number_rows, number_columns) {}

    int number_rows{10};
    int number_columns{5};
    GridLogic grid_logic;
};

TEST_F(GridLogicTest, EmptyGridAtInstantiation) {
    // check whether all grid cells are false
    bool is_occupancy_grid_empty{true};
    auto occupancy_grid = grid_logic.GetOccupancyGrid();
    for (const auto &row : occupancy_grid) {
        for (const bool &is_cell_occupied : row) {
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
    // place a square-shape in the upper left corner
    TetrominoPositionType current_position{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    const TetrominoPositionType& target_position{current_position};

    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestForBottomRightCorner) {
    // place a square-shape in the bottom right corner
    TetrominoPositionType current_position{
        {number_rows - 2, number_columns - 2},
        {number_rows - 1, number_columns - 2},
        {number_rows - 1, number_columns - 1},
        {number_rows - 2, number_columns - 1}};
    const TetrominoPositionType& target_position{current_position};

    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfLeftBounce) {
    // place a square-shape in the upper left corner
    TetrominoPositionType current_position{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    TetrominoPositionType target_position{current_position};
    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step to the left
    target_position = {{0, -1}, {1, -1}, {1, 0}, {0, 0}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_FALSE(actual_result);
}

TEST_F(GridLogicTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfRightBounce) {
    // place a square-shape in the upper right corner
    TetrominoPositionType current_position{{0, number_columns - 2},
                                           {1, number_columns - 2},
                                           {1, number_columns - 1},
                                           {0, number_columns - 1}};
    TetrominoPositionType target_position{current_position};
    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step to the right
    target_position = {{0, number_columns - 1},
                       {1, number_columns - 1},
                       {1, number_columns},
                       {0, number_columns}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_FALSE(actual_result);
}

TEST_F(GridLogicTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfBottomBounce) {
    // place a square-shape in the bottom left corner
    TetrominoPositionType current_position{{number_rows - 2, 0},
                                           {number_rows - 1, 0},
                                           {number_rows - 1, 1},
                                           {number_rows - 2, 1}};
    TetrominoPositionType target_position{current_position};
    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step down
    target_position = {{number_rows - 1, 0},
                       {number_rows, 0},
                       {number_rows, 1},
                       {number_rows - 1, 1}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_FALSE(actual_result);
}

TEST_F(GridLogicTest, UnsuccessfullGridOccupancyRequestBecauseAlreadyOccupied) {
    // place a square-shape in the upper left corner
    TetrominoPositionType current_position{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    const TetrominoPositionType& target_position{current_position};
    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // place a square-shape at the same place it has been before
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_FALSE(actual_result);
}

TEST_F(
    GridLogicTest,
    SuccessfullGridOccupancyRequestWhenMovedRightLeftRightAndLeftAgainTwoStepsEachTime) {
    // place a square-shape in the upper left corner
    TetrominoPositionType current_position{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    TetrominoPositionType target_position{current_position};
    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move 2 steps to the right
    current_position = target_position;
    target_position = {{0, 2}, {1, 2}, {1, 3}, {0, 3}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move 2 steps to the left
    current_position = target_position;
    target_position = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move 2 steps to the right
    current_position = target_position;
    target_position = {{0, 2}, {1, 2}, {1, 3}, {0, 3}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move 2 steps to the left again
    current_position = target_position;
    target_position = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestWhenMovedRightOneStep) {
    // place a square-shape somewhere on the grid
    TetrominoPositionType current_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    TetrominoPositionType target_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step to the right
    target_position = {{3, 2 + 1}, {4, 2 + 1}, {4, 3 + 1}, {3, 3 + 1}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestWhenMovedLeftOneStep) {
    // place a square-shape somewhere on the grid
    TetrominoPositionType current_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    TetrominoPositionType target_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step to the left
    target_position = {{3, 2 - 1}, {4, 2 - 1}, {4, 3 - 1}, {3, 3 - 1}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestWhenMovedDownOneStep) {
    // place a square-shape somewhere on the grid
    TetrominoPositionType current_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    TetrominoPositionType target_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    bool actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step down
    target_position = {{3 + 1, 2}, {4 + 1, 2}, {4 + 1, 3}, {3 + 1, 3}};
    actual_result =
        grid_logic.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}