#include "../src/Tetromino.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using TetrinosCoordinateType =
    std::tuple<std::pair<int, int>, std::pair<int, int>, std::pair<int, int>,
               std::pair<int, int>>;

class GameBackgroundMock{
   public:
    MOCK_METHOD(bool, RequestSpaceOnGrid,
                (TetrinosCoordinateType requested_coordinates));
};

class TetrominoTest : public ::testing::Test {
   protected:
    TetrominoTest() : unit(init_position, unit_color){};
    TetrominoPositionType init_position{{0, 0}, {0, 1}, {0, 2}, {0, 3}};
    Color unit_color{Color::blue};
    GameBackgroundMock background_mock;
    Tetromino unit;
};

TEST_F(TetrominoTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.getPosition();
    Color expected_color{unit_color};
    Color actual_color = unit.getColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
}

// TEST_F(TetrominoTest, MoveToRightPossible) {
//     TetrominoPositionType current_position{init_position};
//     TetrominoPositionType expected_position{init_position};
//     for (auto& elem : expected_position) {
//         elem.second++;
//     }
//     //unit.moveOneStep(Direction::right, background_mock);
//     TetrominoPositionType actual_position = unit.getPosition();
//     EXPECT_EQ(expected_position, actual_position);
// }