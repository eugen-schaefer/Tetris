#include "../src/GameBackground.h"
#include "gtest/gtest.h"

class GameBackgroundTest : public ::testing::Test {
   protected:
    GameBackgroundTest()
        : game_background(vertical_grid_size, horizontal_grid_size) {}
    int horizontal_grid_size{5};
    int vertical_grid_size{10};
    GameBackground game_background;
};

TEST_F(GameBackgroundTest, EmptyGridAtInstantiation) {
    // check whether all grid cells are false
    bool is_occupancy_grid_empty{true};
    auto occupancy_grid = game_background.GetOccupancyGrid();
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

TEST_F(GameBackgroundTest, ZeroFilledButtomLinesAtInstantiation) {
    int expected_nr_buttomlines_filled{};
    EXPECT_EQ(expected_nr_buttomlines_filled,
              game_background.GetNumberFilledBottomLines());
}

TEST_F(GameBackgroundTest, SuccessfullGridOccupancyRequestForTopLeftCorner) {
    GameBackground::TetrinosCoordinateType quadrupel{
        {0, 0}, {1, 0}, {1, 1}, {0, 1}};
    bool actual_result = game_background.RequestSpaceOnGrid(quadrupel);
    EXPECT_TRUE(actual_result);
}

TEST_F(GameBackgroundTest,
       SuccessfullGridOccupancyRequestForBottomRightCorner) {
    int m = vertical_grid_size;
    int n = horizontal_grid_size;
    GameBackground::TetrinosCoordinateType quadrupel{
        {m - 2, n - 2}, {m - 1, n - 2}, {m - 1, n - 1}, {m - 2, n - 1}};
    bool actual_result = game_background.RequestSpaceOnGrid(quadrupel);
    EXPECT_TRUE(actual_result);
}

TEST_F(GameBackgroundTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfLeftBounce) {
    int m = vertical_grid_size;
    int n = horizontal_grid_size;
    GameBackground::TetrinosCoordinateType quadrupel{
        {0, -1}, {1, -1}, {1, 0}, {0, 0}};
    bool actual_result = game_background.RequestSpaceOnGrid(quadrupel);
    EXPECT_FALSE(actual_result);
}

TEST_F(GameBackgroundTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfRightBounce) {
    int m = vertical_grid_size;
    int n = horizontal_grid_size;
    GameBackground::TetrinosCoordinateType quadrupel{
        {0, horizontal_grid_size - 1},
        {1, horizontal_grid_size - 1},
        {1, horizontal_grid_size},
        {0, horizontal_grid_size}};
    bool actual_result = game_background.RequestSpaceOnGrid(quadrupel);
    EXPECT_FALSE(actual_result);
}

TEST_F(GameBackgroundTest,
       UnsuccessfullGridOccupancyRequestBecauseOfOutOfBottomBounce) {
    int m = vertical_grid_size;
    int n = horizontal_grid_size;
    GameBackground::TetrinosCoordinateType quadrupel{
        {vertical_grid_size - 1, 0},
        {vertical_grid_size, 0},
        {vertical_grid_size, 1},
        {vertical_grid_size - 1, 1}};
    bool actual_result = game_background.RequestSpaceOnGrid(quadrupel);
    EXPECT_FALSE(actual_result);
}

TEST_F(GameBackgroundTest,
       UnsuccessfullGridOccupancyRequestBecauseAlreadyOccupied) {
    GameBackground::TetrinosCoordinateType quadrupel{
        {0, 0}, {1, 0}, {1, 1}, {0, 1}};
    bool actual_result = game_background.RequestSpaceOnGrid(quadrupel);
    EXPECT_TRUE(actual_result);
    actual_result = game_background.RequestSpaceOnGrid(quadrupel);
    EXPECT_FALSE(actual_result);
}