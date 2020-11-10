#include "../src/GridLogic.h"
#include "gtest/gtest.h"

class GridLogicTest : public ::testing::Test {
   protected:
    GridLogicTest() : unit(number_rows, number_columns) {}

    void OccupyEntireRow(int row_index) {
        // Occupy the left half of the row
        TetrominoPositionType current_position{
            {row_index, 0}, {row_index, 1}, {row_index, 2}, {row_index, 3}};
        TetrominoPositionType target_position{current_position};
        bool actual_result =
            unit.RequestSpaceOnGrid(current_position, target_position);
        EXPECT_TRUE(actual_result);

        // Occupy the right half of the row
        current_position = {
            {row_index, 4}, {row_index, 5}, {row_index, 6}, {row_index, 7}};
        target_position = current_position;
        actual_result =
            unit.RequestSpaceOnGrid(current_position, target_position);
        EXPECT_TRUE(actual_result);
    };

    int number_rows{10};
    int number_columns{8};
    GridLogic unit;
};

TEST_F(GridLogicTest, EmptyGridAtInstantiation) {
    // check whether all grid cells are false
    bool is_occupancy_grid_empty{true};
    auto occupancy_grid = unit.GetOccupancyGrid();
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

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestForTopLeftCorner) {
    // place a square-shape in the upper left corner
    TetrominoPositionType current_position{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    const TetrominoPositionType& target_position{current_position};

    bool actual_result =
        unit.RequestSpaceOnGrid(current_position, target_position);
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
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfLeftBounce) {
    // place a square-shape in the upper left corner
    TetrominoPositionType current_position{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    TetrominoPositionType target_position{current_position};
    bool actual_result =
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step to the left
    target_position = {{0, -1}, {1, -1}, {1, 0}, {0, 0}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
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
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step to the right
    target_position = {{0, number_columns - 1},
                       {1, number_columns - 1},
                       {1, number_columns},
                       {0, number_columns}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
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
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step down
    target_position = {{number_rows - 1, 0},
                       {number_rows, 0},
                       {number_rows, 1},
                       {number_rows - 1, 1}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_FALSE(actual_result);
}

TEST_F(GridLogicTest, UnsuccessfullGridOccupancyRequestBecauseAlreadyOccupied) {
    // place a square-shape in the upper left corner
    TetrominoPositionType current_position{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    const TetrominoPositionType& target_position{current_position};
    bool actual_result =
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // place a square-shape at the same place it has been before
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_FALSE(actual_result);
}

TEST_F(
    GridLogicTest,
    SuccessfullGridOccupancyRequestWhenMovedRightLeftRightAndLeftAgainTwoStepsEachTime) {
    // place a square-shape in the upper left corner
    TetrominoPositionType current_position{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    TetrominoPositionType target_position{current_position};
    bool actual_result =
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move 2 steps to the right
    current_position = target_position;
    target_position = {{0, 2}, {1, 2}, {1, 3}, {0, 3}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move 2 steps to the left
    current_position = target_position;
    target_position = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move 2 steps to the right
    current_position = target_position;
    target_position = {{0, 2}, {1, 2}, {1, 3}, {0, 3}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move 2 steps to the left again
    current_position = target_position;
    target_position = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestWhenMovedRightOneStep) {
    // place a square-shape somewhere on the grid
    TetrominoPositionType current_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    TetrominoPositionType target_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    bool actual_result =
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step to the right
    target_position = {{3, 2 + 1}, {4, 2 + 1}, {4, 3 + 1}, {3, 3 + 1}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestWhenMovedLeftOneStep) {
    // place a square-shape somewhere on the grid
    TetrominoPositionType current_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    TetrominoPositionType target_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    bool actual_result =
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step to the left
    target_position = {{3, 2 - 1}, {4, 2 - 1}, {4, 3 - 1}, {3, 3 - 1}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, SuccessfullGridOccupancyRequestWhenMovedDownOneStep) {
    // place a square-shape somewhere on the grid
    TetrominoPositionType current_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    TetrominoPositionType target_position{{3, 2}, {4, 2}, {4, 3}, {3, 3}};
    bool actual_result =
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // move the square-shape one step down
    target_position = {{3 + 1, 2}, {4 + 1, 2}, {4 + 1, 3}, {3 + 1, 3}};
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);
}

TEST_F(GridLogicTest, NoOccupiedRowsAtInstantiation) {
    auto vector_of_indexes_of_fully_occupied_rows =
        unit.GetIndexesOfFullyOccupiedRows();
    EXPECT_TRUE(vector_of_indexes_of_fully_occupied_rows.empty());
}

TEST_F(GridLogicTest, FirstBottomRowAlmostOccupied) {
    // Occupy (n-2) cells in the bottom row

    // Occupy the left half of the row
    TetrominoPositionType current_position{{number_rows - 1, 0},
                                           {number_rows - 1, 1},
                                           {number_rows - 1, 2},
                                           {number_rows - 1, 3}};
    TetrominoPositionType target_position{current_position};
    bool actual_result =
        unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    // Occupy the left half of the row
    current_position = {{number_rows - 1, 4},
                        {number_rows - 1, 5},
                        {number_rows - 1, 6},
                        {number_rows - 2, 6}};
    target_position = current_position;
    actual_result = unit.RequestSpaceOnGrid(current_position, target_position);
    EXPECT_TRUE(actual_result);

    auto vector_of_indexes_of_fully_occupied_rows =
        unit.GetIndexesOfFullyOccupiedRows();
    EXPECT_TRUE(vector_of_indexes_of_fully_occupied_rows.empty());
}

TEST_F(GridLogicTest, FirstBottomRowOccupied) {
    // Occupy the first bottom row
    OccupyEntireRow(number_rows - 1);

    auto vector_of_indexes_of_fully_occupied_rows =
        unit.GetIndexesOfFullyOccupiedRows();
    EXPECT_FALSE(vector_of_indexes_of_fully_occupied_rows.empty());

    EXPECT_EQ(1, vector_of_indexes_of_fully_occupied_rows.size());
    EXPECT_EQ(number_rows - 1, vector_of_indexes_of_fully_occupied_rows.at(0));
}

TEST_F(GridLogicTest, FirstAndSecondBottomRowOccupied) {
    // Occupy the first and the second bottom row
    OccupyEntireRow(number_rows - 1);
    OccupyEntireRow(number_rows - 2);

    auto vector_of_indexes_of_fully_occupied_rows =
        unit.GetIndexesOfFullyOccupiedRows();
    EXPECT_FALSE(vector_of_indexes_of_fully_occupied_rows.empty());

    EXPECT_EQ(2, vector_of_indexes_of_fully_occupied_rows.size());
    EXPECT_EQ(number_rows - 1, vector_of_indexes_of_fully_occupied_rows.at(0));
    EXPECT_EQ(number_rows - 2, vector_of_indexes_of_fully_occupied_rows.at(1));
}

TEST_F(GridLogicTest, FirstAndThirdBottomRowOccupied) {
    // Occupy the first and the third bottom row
    OccupyEntireRow(number_rows - 1);
    OccupyEntireRow(number_rows - 3);

    auto vector_of_indexes_of_fully_occupied_rows =
        unit.GetIndexesOfFullyOccupiedRows();
    EXPECT_FALSE(vector_of_indexes_of_fully_occupied_rows.empty());

    EXPECT_EQ(2, vector_of_indexes_of_fully_occupied_rows.size());
    EXPECT_EQ(number_rows - 1, vector_of_indexes_of_fully_occupied_rows.at(0));
    EXPECT_EQ(number_rows - 3, vector_of_indexes_of_fully_occupied_rows.at(1));
}

TEST_F(GridLogicTest, SecondAndFifthBottomRowOccupied) {
    // Occupy the second and the fifth bottom row
    OccupyEntireRow(number_rows - 2);
    OccupyEntireRow(number_rows - 5);

    auto vector_of_indexes_of_fully_occupied_rows =
        unit.GetIndexesOfFullyOccupiedRows();
    EXPECT_FALSE(vector_of_indexes_of_fully_occupied_rows.empty());

    EXPECT_EQ(2, vector_of_indexes_of_fully_occupied_rows.size());
    EXPECT_EQ(number_rows - 2, vector_of_indexes_of_fully_occupied_rows.at(0));
    EXPECT_EQ(number_rows - 5, vector_of_indexes_of_fully_occupied_rows.at(1));
}
