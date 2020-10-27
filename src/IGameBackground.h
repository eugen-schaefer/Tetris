#ifndef I_GAME_BACKGROUND_H_
#define I_GAME_BACKGROUND_H_

// #include <tuple>
#include <utility>
#include <vector>

#include <iostream>

using TetrominoPositionType = std::vector<std::pair<int, int>>;

class IGameBackground {
   public:
    virtual ~IGameBackground() = default;
    virtual std::vector<std::vector<bool>> GetOccupancyGrid() const = 0;
    virtual int GetNumberFilledBottomLines() const = 0;
    virtual bool RequestSpaceOnGrid(TetrominoPositionType) = 0;
};

#endif /* I_GAME_BACKGROUND_H_ */