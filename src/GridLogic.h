#ifndef GRID_LOGIC_H_
#define GRID_LOGIC_H_

#include <tuple>
#include <utility>
#include <vector>

#include "IGridLogic.h"

/// The GridLogic class keeps track of the playfield, which is a grid into which
/// the tetrominoes are falling. So the class determines whether the active
/// figure can be moved or rotated in the grid or not. The associated grid is
/// also referred to as the logical grid. The x-axis is vertical and counts
/// positive from top to bottom whereas the y-axis is horizontal counting
/// positive from left to right.
class GridLogic : public IGridLogic {
   public:
    /// Constructs a playfield as a grid
    /// \param number_rows:    number of rows in the grid being constructed
    /// \param number_columns: number of columns in the grid eing constructed
    GridLogic(int number_rows, int number_columns);

    /// Retrieves the grid
    /// \return logical grid
    std::vector<std::vector<bool>> GetOccupancyGrid() const;

    /// Retrieves the number of those rows in the grid which are fully occupied
    /// by tetrominoes.
    /// \return: a list of fully occupied rows
    std::vector<int> GetIndexesOfFullyOccupiedRows() const;

    /// Requests specified cells in the grid. If any requested cell is already
    /// occupied by former requests, the current request is rejected. In all
    /// other cases, the request is granted and the requested cells are marked
    /// as occupied/booked. This method is supposed to be used by the single
    /// tetrominoes which in turn are requested to be moved either by the player
    /// via the keyboard event in any direction or periodically by the
    /// controller in the down direction.
    /// \param current_position: Position from which to move the figure.
    /// \param target_position:  Position to which to move the figure.
    /// \return true when request has been granted, false in case of rejection.
    bool RequestSpaceOnGrid(TetrominoPositionType current_position,
                            TetrominoPositionType target_position) override;

    /// Frees all lines which are fully occupied by tetrominoes. This method is
    /// supposed to be used when fully occupied lines are cleared.
    void FreeAllEntirelyOccupiedRows();

    /// Frees all cells unconditionally. This method is supposed to be used in
    /// case of game over to reset the game.
    void FreeEntireGrid();

   private:
    int m_number_columns{};
    int m_number_rows{};

    // vector containing the indexes of entirely occupied rows in
    // m_occupancy_grid in the order from bottom to top, i.e.
    // m_indexes_of_fully_occupied_rows[0] corresponds to the lowest entirely
    // occupied row whereas m_indexes_of_fully_occupied_rows[size-1] corresponds
    // to the most top entirely occupied row
    std::vector<int> m_indexes_of_fully_occupied_rows{};

    // The grid is rectangular, i.e. it consists of m x n cells, where m is the
    // number of rows and n is the number of columns. Columns are numbered from
    // left to right, and rows - unconventionally - from top to bottom. Each
    // grid cell is clearly specified by a coordinate. So the grid cell at the
    // top left has the coordinate (0/0), while the grid cell at the bottom
    // right has the coordinate (m-1 / n-1). Consequently, the grid cell at the
    // bottom left is identified by the coordinate (m-1/0), while the grid cell
    // at the top right is defined by the coordinate (0 / n-1).
    std::vector<std::vector<bool>> m_occupancy_grid{};
};

#endif /* GRID_LOGIC_H_ */
