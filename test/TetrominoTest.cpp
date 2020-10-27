#include "../src/IGameBackground.h"
#include "../src/Tetromino.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class GameBackgroundMock : public IGameBackground {
   public:
    GameBackgroundMock() : IGameBackground() {
    };
    MOCK_METHOD((std::vector<std::vector<bool>>), GetOccupancyGrid, (),
                (const));
    MOCK_METHOD(int, GetNumberFilledBottomLines, (), (const));
    MOCK_METHOD(bool, RequestSpaceOnGrid,
                (TetrominoPositionType requested_coordinates), (override));
};

class TetrominoTest : public ::testing::Test {
   protected:
    TetrominoTest() : unit(game_background_mock, init_position, unit_color){};
    TetrominoPositionType init_position{{0, 0}, {0, 1}, {0, 2}, {0, 3}};

    Color unit_color{Color::blue};
    Tetromino unit;
    GameBackgroundMock game_background_mock;
};

TEST_F(TetrominoTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.getPosition();
    Color expected_color{unit_color};
    Color actual_color = unit.getColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
}

TEST_F(TetrominoTest, MoveToRightPossible) {
    TetrominoPositionType current_position{init_position};
    TetrominoPositionType expected_position{init_position};
    for (auto& elem : expected_position) {
        elem.second++;
    }

    ON_CALL(game_background_mock, RequestSpaceOnGrid(current_position))
        .WillByDefault(::testing::Return(true));

    unit.moveOneStep(Direction::right);
    TetrominoPositionType actual_position = unit.getPosition();
    EXPECT_EQ(expected_position, actual_position);
}