#ifndef I_GRID_LOGIC_H_
#define I_GRID_LOGIC_H_

#include <iostream>
#include <utility>
#include <vector>

using TetrominoPositionType = std::vector<std::pair<int, int>>;

class IGridLogic {
   public:
    virtual ~IGridLogic() = default;
    virtual std::vector<std::vector<bool>> GetOccupancyGrid() const = 0;
    virtual int GetNumberFilledBottomLines() const = 0;
    virtual bool RequestSpaceOnGrid(TetrominoPositionType current_position,
                                    TetrominoPositionType target_position) = 0;
};

#endif /* I_GRID_LOGIC_H_ */