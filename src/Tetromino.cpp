#include "Tetromino.h"

Tetromino::Tetromino(TetrominoPositionType init_position, Color color)
    : m_position{init_position}, m_color{color} {};

Color Tetromino::getColor() { return m_color; }

TetrominoPositionType Tetromino::getPosition() { return m_position; }

void Tetromino::moveOneStep(Direction, GameBackground& bg) {}
