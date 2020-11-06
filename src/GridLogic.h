#ifndef GRID_LOGIC_H_
#define GRID_LOGIC_H_

#include <tuple>
#include <utility>
#include <vector>

#include "IGridLogic.h"

class GridLogic : public IGridLogic {
   public:
    GridLogic(int, int);

    std::vector<std::vector<bool>> GetOccupancyGrid() const override;
    int GetNumberFilledBottomLines() const override;
    bool RequestSpaceOnGrid(TetrominoPositionType) override;

   private:
    int m_horizontal_grid_size;
    int m_vertical_grid_size;
    int m_nr_buttomlines_filled{};

    // The grid is rectangular, i.e. it consists of m x n cells, where m is the
    // number of rows and n is the number of columns. Each grid cell is clearly
    // specified by a coordinate. The grid cell at the top left has the
    // coordinate (0/0), while the grid cell at the bottom right has the
    // coordinate (m-1 / n-1). Consequently, the grid cell at the bottom left is
    // identified by the coordinate (m-1/0), while the grid cell at the top
    // right is defined by the coordinate (0 / n-1).
    std::vector<std::vector<bool>> m_occupancy_grid{};
};

#endif /* GRID_LOGIC_H_ */