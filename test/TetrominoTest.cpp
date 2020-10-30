#include "../src/IGameBackground.h"
#include "../src/Tetromino.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class GameBackgroundMock : public IGameBackground {
   public:
    GameBackgroundMock() : IGameBackground(){};
    MOCK_METHOD((std::vector<std::vector<bool>>), GetOccupancyGrid, (),
                (const));
    MOCK_METHOD(int, GetNumberFilledBottomLines, (), (const));
    MOCK_METHOD(bool, RequestSpaceOnGrid,
                (TetrominoPositionType requested_coordinates), (override));
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the Tetromino ----------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

class TetrominoTest : public ::testing::Test {
   protected:
    bool const kTargetPositionFree{true};
    bool const kTargetPositionOccupied{false};
    TetrominoTest() : unit{game_background_mock, init_position, unit_color} {};
    TetrominoPositionType init_position{{0, 0}, {0, 1}, {0, 2}, {0, 3}};

    Color const unit_color{Color::blue};
    Tetromino unit;
    ::testing::NiceMock<GameBackgroundMock> game_background_mock;

    TetrominoPositionType moveTetrominoIfPossible(
        TetrominoPositionType initial_position, Direction direction,
        bool is_free) {
        TetrominoPositionType expected_position{initial_position};

        for (auto& elem : expected_position) {
            switch (direction) {
                case Direction::left:
                    --elem.second;
                    break;
                case Direction::right:
                    ++elem.second;
                    break;
                case Direction::down:
                    ++elem.first;
                    break;
                default:
                    break;
            }
        }

        ON_CALL(game_background_mock, RequestSpaceOnGrid(expected_position))
            .WillByDefault(::testing::Return(is_free));

        unit.moveOneStep(direction);

        return expected_position;
    }
};

TEST_F(TetrominoTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.getPosition();
    Color expected_color{unit_color};
    Color actual_color = unit.getColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
}

TEST_F(TetrominoTest, GetColorOfTetromino) {
    Tetromino red_tetromino{game_background_mock, init_position, Color::red};
    EXPECT_EQ(Color::red, red_tetromino.getColor());
}

TEST_F(TetrominoTest, Positioning) {
    TetrominoPositionType start_position{{5, 1}, {5, 2}, {5, 3}, {5, 4}};
    TetrominoPositionType target_position{{6, 1}, {6, 2}, {6, 3}, {6, 4}};
    Tetromino tetromino{game_background_mock, start_position, unit_color};
    ON_CALL(game_background_mock, RequestSpaceOnGrid(target_position))
        .WillByDefault(::testing::Return(true));
    tetromino.moveOneStep(Direction::down);
    EXPECT_EQ(target_position, tetromino.getPosition());
}

TEST_F(TetrominoTest, MoveToTheRightPossible) {
    auto expected_position = moveTetrominoIfPossible(
        init_position, Direction::right, kTargetPositionFree);
    TetrominoPositionType actual_position = unit.getPosition();
    EXPECT_EQ(expected_position, actual_position);
}

TEST_F(TetrominoTest, MoveToTheRightImpossibleBecauseOfOccupiedTargetRegion) {
    auto expected_position = moveTetrominoIfPossible(
        init_position, Direction::right, kTargetPositionOccupied);
    TetrominoPositionType actual_position = unit.getPosition();
    EXPECT_NE(expected_position, actual_position);
}

TEST_F(TetrominoTest, MoveToTheLeftPossible) {
    auto expected_position = moveTetrominoIfPossible(
        init_position, Direction::left, kTargetPositionFree);
    TetrominoPositionType actual_position = unit.getPosition();
    EXPECT_EQ(expected_position, actual_position);
}

TEST_F(TetrominoTest, MoveToTheLeftImpossibleBecauseOfOccupiedTargetRegion) {
    auto expected_position = moveTetrominoIfPossible(
        init_position, Direction::left, kTargetPositionOccupied);
    TetrominoPositionType actual_position = unit.getPosition();
    EXPECT_NE(expected_position, actual_position);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the I-Shape ------------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

class ShapeITest : public ::testing::Test {
   protected:
    bool const kTargetPositionFree{true};
    bool const kTargetPositionOccupied{false};
    ShapeITest() : unit{game_background_mock, init_position, unit_color} {};
    TetrominoPositionType init_position{{6, 1}, {6, 2}, {6, 3}, {6, 4}};

    Color const unit_color{Color::cyan};
    ShapeI unit;
    ::testing::NiceMock<GameBackgroundMock> game_background_mock;
};

TEST_F(ShapeITest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.getPosition();
    Color expected_color{unit_color};
    Color actual_color = unit.getColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
    EXPECT_EQ(Orientation::north, unit.getOrientation());
}

TEST_F(ShapeITest, RotateClockwiseOnce) {
    TetrominoPositionType expected_position{{4, 3}, {5, 3}, {6, 3}, {7, 3}};

    ON_CALL(game_background_mock, RequestSpaceOnGrid(expected_position))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();

    EXPECT_EQ(Orientation::east, unit.getOrientation());
    EXPECT_EQ(expected_position, unit.getPosition());
}
