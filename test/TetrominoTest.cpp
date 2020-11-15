#include "../src/IGridLogic.h"
#include "../src/Tetromino.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class GridLogicMock : public IGridLogic {
   public:
    GridLogicMock() : IGridLogic(){};
    MOCK_METHOD(bool, RequestSpaceOnGrid,
                (TetrominoPositionType current_position,
                 TetrominoPositionType target_position),
                (override));
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the Tetromino ----------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

class TetrominoTest : public ::testing::Test {
   protected:
    bool const kTargetPositionFree{true};
    bool const kTargetPositionOccupied{false};

    TetrominoTest() : unit{grid_logic_mock, init_position, unit_color} {};
    TetrominoPositionType init_position{{0, 0}, {0, 1}, {0, 2}, {0, 3}};

    Color const unit_color{Color::blue};
    Tetromino unit;
    ::testing::NiceMock<GridLogicMock> grid_logic_mock;

    struct MoveTetrominoReturnValueType {
        bool is_move_successfull;
        TetrominoPositionType expected_targed_position;
    };
    MoveTetrominoReturnValueType MoveTetrominoIfPossible(
        const TetrominoPositionType& current_position, Direction direction,
        bool is_free) {
        TetrominoPositionType target_position{current_position};

        MoveTetrominoReturnValueType returnValue;
        returnValue.is_move_successfull = false;
        returnValue.expected_targed_position = current_position;
        if (!unit.IsMovable()) {
            return returnValue;
        }

        for (auto& elem : target_position) {
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

        ON_CALL(grid_logic_mock,
                RequestSpaceOnGrid(current_position, target_position))
            .WillByDefault(::testing::Return(is_free));

        returnValue.is_move_successfull = unit.MoveOneStep(direction);
        returnValue.expected_targed_position = target_position;

        return returnValue;
    }
};

TEST_F(TetrominoTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.GetPosition();
    Color expected_color{unit_color};
    Color actual_color = unit.GetColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
}

TEST_F(TetrominoTest, GetColorOfTetromino) {
    Tetromino red_tetromino{grid_logic_mock, init_position, Color::red};
    EXPECT_EQ(Color::red, red_tetromino.GetColor());
}

TEST_F(TetrominoTest, MoveToTheRightPossible) {
    auto result = MoveTetrominoIfPossible(init_position, Direction::right,
                                          kTargetPositionFree);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_TRUE(result.is_move_successfull);
    EXPECT_EQ(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveToTheRightImpossibleBecauseOfOccupiedTargetRegion) {
    auto result = MoveTetrominoIfPossible(init_position, Direction::right,
                                          kTargetPositionOccupied);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_FALSE(result.is_move_successfull);
    EXPECT_NE(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveToTheRightImpossibleBecauseOfMovabilityFreeze) {
    unit.MakeUnmovable();
    auto result = MoveTetrominoIfPossible(init_position, Direction::right,
                                          kTargetPositionFree);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_FALSE(result.is_move_successfull);
    EXPECT_EQ(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveToTheLeftPossible) {
    auto result = MoveTetrominoIfPossible(init_position, Direction::left,
                                          kTargetPositionFree);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_TRUE(result.is_move_successfull);
    EXPECT_EQ(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveToTheLeftImpossibleBecauseOfOccupiedTargetRegion) {
    auto result = MoveTetrominoIfPossible(init_position, Direction::left,
                                          kTargetPositionOccupied);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_FALSE(result.is_move_successfull);
    EXPECT_NE(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveToTheLeftImpossibleBecauseOfMovabilityFreeze) {
    unit.MakeUnmovable();
    auto result = MoveTetrominoIfPossible(init_position, Direction::left,
                                          kTargetPositionFree);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_FALSE(result.is_move_successfull);
    EXPECT_EQ(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveDownPossible) {
    auto result = MoveTetrominoIfPossible(init_position, Direction::down,
                                          kTargetPositionFree);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_TRUE(result.is_move_successfull);
    EXPECT_EQ(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveDownImpossibleBecauseOfOccupiedTargetRegion) {
    auto result = MoveTetrominoIfPossible(init_position, Direction::down,
                                          kTargetPositionOccupied);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_FALSE(result.is_move_successfull);
    EXPECT_NE(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveDownImpossibleBecauseOfMovabilityFreeze) {
    unit.MakeUnmovable();
    auto result = MoveTetrominoIfPossible(init_position, Direction::down,
                                          kTargetPositionFree);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_FALSE(result.is_move_successfull);
    EXPECT_EQ(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, DeleteFirstSquareElement) {
    TetrominoPositionType expected_pos_after_deletion{{0, 1}, {0, 2}, {0, 3}};
    auto iterator_to_square_to_be_deleted =
        unit.GetIteratorToBeginOfPositionVector();
    unit.DeleteTetrominoSquare(iterator_to_square_to_be_deleted);
    TetrominoPositionType actual_position_after_deletion = unit.GetPosition();
    EXPECT_EQ(expected_pos_after_deletion, actual_position_after_deletion);
}

TEST_F(TetrominoTest, DeleteSecondSquareElement) {
    TetrominoPositionType expected_pos_after_deletion{{0, 0}, {0, 2}, {0, 3}};
    auto iterator_to_square_to_be_deleted =
        unit.GetIteratorToBeginOfPositionVector() + 1;
    unit.DeleteTetrominoSquare(iterator_to_square_to_be_deleted);
    TetrominoPositionType actual_position_after_deletion = unit.GetPosition();
    EXPECT_EQ(expected_pos_after_deletion, actual_position_after_deletion);
}

TEST_F(TetrominoTest, DeleteThirdSquareElement) {
    TetrominoPositionType expected_pos_after_deletion{{0, 0}, {0, 1}, {0, 3}};
    auto iterator_to_square_to_be_deleted =
        unit.GetIteratorToBeginOfPositionVector() + 2;
    unit.DeleteTetrominoSquare(iterator_to_square_to_be_deleted);
    TetrominoPositionType actual_position_after_deletion = unit.GetPosition();
    EXPECT_EQ(expected_pos_after_deletion, actual_position_after_deletion);
}

TEST_F(TetrominoTest, DeleteForthSquareElement) {
    TetrominoPositionType expected_pos_after_deletion{{0, 0}, {0, 1}, {0, 2}};
    auto iterator_to_square_to_be_deleted =
        unit.GetIteratorToBeginOfPositionVector() + 3;
    unit.DeleteTetrominoSquare(iterator_to_square_to_be_deleted);
    TetrominoPositionType actual_position_after_deletion = unit.GetPosition();
    EXPECT_EQ(expected_pos_after_deletion, actual_position_after_deletion);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the I-Shape ------------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

class ShapeITest : public ::testing::Test {
   protected:
    ShapeITest() : unit{grid_logic_mock} {
        ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(true));
        unit.MoveOneStep(Direction::down);
        unit.MoveOneStep(Direction::down);
        unit.MoveOneStep(Direction::down);
        unit.MoveOneStep(Direction::down);
        unit.MoveOneStep(Direction::down);
        unit.MoveOneStep(Direction::down);
        unit.MoveOneStep(Direction::right);
    };

    TetrominoPositionType init_position{{6, 1}, {6, 2}, {6, 3}, {6, 4}};
    ShapeI unit;
    ::testing::NiceMock<GridLogicMock> grid_logic_mock;
};

TEST_F(ShapeITest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.GetPosition();
    Color expected_color{Color::cyan};
    Color actual_color = unit.GetColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
    EXPECT_EQ(Orientation::north, unit.GetOrientation());
}

TEST_F(ShapeITest, RotateClockwiseOnce) {
    TetrominoPositionType expected_position{{4, 3}, {5, 3}, {6, 3}, {7, 3}};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();

    EXPECT_EQ(Orientation::east, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeITest, RotateClockwiseOnceNotPossibleBecauseOfMovabilityFreeze) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.MakeUnmovable();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeITest, RotateClockwiseTwice) {
    TetrominoPositionType expected_position{{6, 4}, {6, 3}, {6, 2}, {6, 1}};

    // "::testing::_" means that the mock will always return what has been
    // specified in the Return statement regardless of the input provided
    // to the mocked RequestSpaceOnGrid
    ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();
    unit.Rotate();

    EXPECT_EQ(Orientation::south, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeITest, RotateClockwiseOnceThreeTimes) {
    TetrominoPositionType expected_position{{7, 2}, {6, 2}, {5, 2}, {4, 2}};

    // "::testing::_" means that the mock will always return what has been
    // specified in the Return statement regardless of the input provided
    // to the mocked RequestSpaceOnGrid
    ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();
    unit.Rotate();
    unit.Rotate();

    EXPECT_EQ(Orientation::west, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeITest, RotateClockwiseOnceFourTimes) {
    TetrominoPositionType expected_position{init_position};

    // "::testing::_" means that the mock will always return what has been
    // specified in the Return statement regardless of the input provided
    // to the mocked RequestSpaceOnGrid
    ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();
    unit.Rotate();
    unit.Rotate();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}
