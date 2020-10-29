#ifndef TETROMINO_H_
#define TETROMINO_H_

#include <memory>
#include <utility>
#include <vector>

#include "IGameBackground.h"

enum class Direction { left, right, down };

enum class Color {
    cyan,    // (0x00FFFF)
    blue,    // (0x0000FF)
    orange,  // (0xFF8040)
    yellow,  // (0xFFFF00)
    green,   // (0x00FF00)
    purple,  // (0x800080)
    red      // (0xFF0000)
};

enum class TetrominoType { I, O, T, J, L, S, Z };

using TetrominoPositionType = std::vector<std::pair<int, int>>;

class Tetromino {
   public:
    Tetromino() = delete;
    Tetromino(IGameBackground&, TetrominoPositionType, Color);
    Color getColor();
    TetrominoPositionType getPosition();
    void moveOneStep(Direction);

   private:
    void setPosition(TetrominoPositionType);
    Color m_color;
    TetrominoPositionType m_position;
    IGameBackground& m_game_background;
};

#endif /* TETROMINO_H_ */