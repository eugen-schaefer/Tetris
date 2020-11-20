#include "../src/IGridLogic.h"
#include "../src/Tetromino.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(TetrominoPositionTypeOperatorOverloading, SuccessWhileAddingTwoPositions) {
    TetrominoPositionType operand1{{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    TetrominoPositionType operand2{{9, 8}, {7, 6}, {5, 4}, {3, 2}};
    TetrominoPositionType expected_addition_result{
        {10, 10}, {10, 10}, {10, 10}, {10, 10}};
    TetrominoPositionType actual_addition_result{operand1 + operand2};
    EXPECT_EQ(expected_addition_result, actual_addition_result);
}

TEST(TetrominoPositionTypeOperatorOverloadingDeathTest,
     NoSuccessWhileAddingTwoPositionsBecauseOfDifferentDimensions) {
    TetrominoPositionType operand1{{1, 2}, {7, 8}};
    TetrominoPositionType operand2{{9, 8}, {7, 6}, {5, 4}};
    EXPECT_DEATH(operand1 + operand2, "");
}

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
        if (unit.IsLocked()) {
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

TEST_F(TetrominoTest, MoveToTheRightImpossibleBecauseOfLockDown) {
    unit.LockDown();
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

TEST_F(TetrominoTest, MoveToTheLeftImpossibleBecauseOfLockDown) {
    unit.LockDown();
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

TEST_F(TetrominoTest, MoveDownImpossibleBecauseOfLockDown) {
    unit.LockDown();
    auto result = MoveTetrominoIfPossible(init_position, Direction::down,
                                          kTargetPositionFree);
    TetrominoPositionType actual_position = unit.GetPosition();
    EXPECT_FALSE(result.is_move_successfull);
    EXPECT_EQ(result.expected_targed_position, actual_position);
}

TEST_F(TetrominoTest, MoveDownPossibleBecauseOfUnlocking) {
    unit.LockDown();
    EXPECT_TRUE(unit.IsLocked());

    unit.Release();
    EXPECT_FALSE(unit.IsLocked());
    auto result = MoveTetrominoIfPossible(init_position, Direction::down,
                                          kTargetPositionFree);
    TetrominoPositionType actual_position = unit.GetPosition();
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
    ShapeITest() : unit{grid_logic_mock, init_position} {
        ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(true));
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
    EXPECT_EQ(TetrominoType::I, unit.GetTetrominoType());
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

TEST_F(ShapeITest, RotateClockwiseOnceNotPossibleBecauseOfLockDown) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.LockDown();
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

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the J-Shape ------------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
class ShapeJTest : public ::testing::Test {
   protected:
    ShapeJTest() : unit{grid_logic_mock, init_position} {
        ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(true));
    };

    TetrominoPositionType init_position{{6, 1}, {7, 1}, {7, 2}, {7, 3}};
    ShapeJ unit;
    ::testing::NiceMock<GridLogicMock> grid_logic_mock;
};

TEST_F(ShapeJTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.GetPosition();
    Color expected_color{Color::blue};
    Color actual_color = unit.GetColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(TetrominoType::J, unit.GetTetrominoType());
}

TEST_F(ShapeJTest, RotateClockwiseOnce) {
    TetrominoPositionType expected_position{{6, 3}, {6, 2}, {7, 2}, {8, 2}};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();

    EXPECT_EQ(Orientation::east, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeJTest, RotateClockwiseOnceNotPossibleBecauseOfLockDown) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.LockDown();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeJTest, RotateClockwiseTwice) {
    TetrominoPositionType expected_position{{8, 3}, {7, 3}, {7, 2}, {7, 1}};

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

TEST_F(ShapeJTest, RotateClockwiseOnceThreeTimes) {
    TetrominoPositionType expected_position{{8, 1}, {8, 2}, {7, 2}, {6, 2}};

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

TEST_F(ShapeJTest, RotateClockwiseOnceFourTimes) {
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

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the L-Shape ------------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
class ShapeLTest : public ::testing::Test {
   protected:
    ShapeLTest() : unit{grid_logic_mock, init_position} {
        ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(true));
    };

    TetrominoPositionType init_position{{6, 3}, {7, 1}, {7, 2}, {7, 3}};
    ShapeL unit;
    ::testing::NiceMock<GridLogicMock> grid_logic_mock;
};

TEST_F(ShapeLTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.GetPosition();
    Color expected_color{Color::orange};
    Color actual_color = unit.GetColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(TetrominoType::L, unit.GetTetrominoType());
}

TEST_F(ShapeLTest, RotateClockwiseOnce) {
    TetrominoPositionType expected_position{{8, 3}, {6, 2}, {7, 2}, {8, 2}};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();

    EXPECT_EQ(Orientation::east, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeLTest, RotateClockwiseOnceNotPossibleBecauseOfLockDown) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.LockDown();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeLTest, RotateClockwiseTwice) {
    TetrominoPositionType expected_position{{8, 1}, {7, 3}, {7, 2}, {7, 1}};

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

TEST_F(ShapeLTest, RotateClockwiseOnceThreeTimes) {
    TetrominoPositionType expected_position{{6, 1}, {6, 2}, {7, 2}, {8, 2}};

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

TEST_F(ShapeLTest, RotateClockwiseOnceFourTimes) {
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

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the O-Shape ------------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
class ShapeOTest : public ::testing::Test {
   protected:
    ShapeOTest() : unit{grid_logic_mock, init_position} {
        ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(true));
    };

    TetrominoPositionType init_position{{6, 2}, {6, 3}, {7, 3}, {7, 2}};
    ShapeO unit;
    ::testing::NiceMock<GridLogicMock> grid_logic_mock;
};

TEST_F(ShapeOTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.GetPosition();
    Color expected_color{Color::yellow};
    Color actual_color = unit.GetColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(TetrominoType::O, unit.GetTetrominoType());
}

TEST_F(ShapeOTest, RotateClockwiseOnce) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeOTest, RotateClockwiseOnceNotPossibleBecauseOfLockDown) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.LockDown();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeOTest, RotateClockwiseTwice) {
    TetrominoPositionType expected_position{init_position};

    // "::testing::_" means that the mock will always return what has been
    // specified in the Return statement regardless of the input provided
    // to the mocked RequestSpaceOnGrid
    ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeOTest, RotateClockwiseOnceThreeTimes) {
    TetrominoPositionType expected_position{init_position};

    // "::testing::_" means that the mock will always return what has been
    // specified in the Return statement regardless of the input provided
    // to the mocked RequestSpaceOnGrid
    ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();
    unit.Rotate();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeOTest, RotateClockwiseOnceFourTimes) {
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

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the S-Shape ------------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
class ShapeSTest : public ::testing::Test {
   protected:
    ShapeSTest() : unit{grid_logic_mock, init_position} {
        ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(true));
    };

    TetrominoPositionType init_position{{6, 2}, {6, 3}, {7, 2}, {7, 1}};
    ShapeS unit;
    ::testing::NiceMock<GridLogicMock> grid_logic_mock;
};

TEST_F(ShapeSTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.GetPosition();
    Color expected_color{Color::green};
    Color actual_color = unit.GetColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(TetrominoType::S, unit.GetTetrominoType());
}

TEST_F(ShapeSTest, RotateClockwiseOnce) {
    TetrominoPositionType expected_position{{7, 3}, {8, 3}, {7, 2}, {6, 2}};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();

    EXPECT_EQ(Orientation::east, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeSTest, RotateClockwiseOnceNotPossibleBecauseOfLockDown) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.LockDown();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeSTest, RotateClockwiseTwice) {
    TetrominoPositionType expected_position{{8, 2}, {8, 1}, {7, 2}, {7, 3}};

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

TEST_F(ShapeSTest, RotateClockwiseOnceThreeTimes) {
    TetrominoPositionType expected_position{{7, 1}, {6, 1}, {7, 2}, {8, 2}};

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

TEST_F(ShapeSTest, RotateClockwiseOnceFourTimes) {
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

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the T-Shape ------------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
class ShapeTTest : public ::testing::Test {
   protected:
    ShapeTTest() : unit{grid_logic_mock, init_position} {
        ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(true));
    };

    TetrominoPositionType init_position{{6, 2}, {7, 1}, {7, 2}, {7, 3}};
    ShapeT unit;
    ::testing::NiceMock<GridLogicMock> grid_logic_mock;
};

TEST_F(ShapeTTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.GetPosition();
    Color expected_color{Color::magenta};
    Color actual_color = unit.GetColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(TetrominoType::T, unit.GetTetrominoType());
}

TEST_F(ShapeTTest, RotateClockwiseOnce) {
    TetrominoPositionType expected_position{{6, 2}, {7, 3}, {7, 2}, {8, 2}};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();

    EXPECT_EQ(Orientation::east, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeTTest, RotateClockwiseOnceNotPossibleBecauseOfLockDown) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.LockDown();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeTTest, RotateClockwiseTwice) {
    TetrominoPositionType expected_position{{8, 2}, {7, 3}, {7, 2}, {7, 1}};

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

TEST_F(ShapeTTest, RotateClockwiseOnceThreeTimes) {
    TetrominoPositionType expected_position{{7, 1}, {8, 2}, {7, 2}, {6, 2}};

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

TEST_F(ShapeTTest, RotateClockwiseOnceFourTimes) {
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

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
// --------------- Tests for the Z-Shape ------------------- //
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
class ShapeZTest : public ::testing::Test {
   protected:
    ShapeZTest() : unit{grid_logic_mock, init_position} {
        ON_CALL(grid_logic_mock, RequestSpaceOnGrid(::testing::_, ::testing::_))
            .WillByDefault(::testing::Return(true));
    };

    TetrominoPositionType init_position{{6, 1}, {6, 2}, {7, 2}, {7, 3}};
    ShapeZ unit;
    ::testing::NiceMock<GridLogicMock> grid_logic_mock;
};

TEST_F(ShapeZTest, Initialization) {
    TetrominoPositionType expected_position{init_position};
    TetrominoPositionType actual_position = unit.GetPosition();
    Color expected_color{Color::red};
    Color actual_color = unit.GetColor();
    EXPECT_EQ(expected_color, actual_color);
    EXPECT_EQ(expected_position, actual_position);
    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(TetrominoType::Z, unit.GetTetrominoType());
}

TEST_F(ShapeZTest, RotateClockwiseOnce) {
    TetrominoPositionType expected_position{{6, 3}, {7, 3}, {7, 2}, {8, 2}};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.Rotate();

    EXPECT_EQ(Orientation::east, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeZTest, RotateClockwiseOnceNotPossibleBecauseOfLockDown) {
    TetrominoPositionType expected_position{init_position};

    ON_CALL(grid_logic_mock,
            RequestSpaceOnGrid(init_position, expected_position))
        .WillByDefault(::testing::Return(true));

    unit.LockDown();
    unit.Rotate();

    EXPECT_EQ(Orientation::north, unit.GetOrientation());
    EXPECT_EQ(expected_position, unit.GetPosition());
}

TEST_F(ShapeZTest, RotateClockwiseTwice) {
    TetrominoPositionType expected_position{{8, 3}, {8, 2}, {7, 2}, {7, 1}};

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

TEST_F(ShapeZTest, RotateClockwiseOnceThreeTimes) {
    TetrominoPositionType expected_position{{8, 1}, {7, 1}, {7, 2}, {6, 2}};

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

TEST_F(ShapeZTest, RotateClockwiseOnceFourTimes) {
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
