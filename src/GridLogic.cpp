#include "GridLogic.h"

GridLogic::GridLogic(int number_rows, int number_columns)
    : m_number_rows{number_rows}, m_number_columns{number_columns} {
    m_occupancy_grid.resize(number_rows);
    for (auto &row : m_occupancy_grid) {
        row.resize(number_columns);
    }
}

std::vector<std::vector<bool>> GridLogic::GetOccupancyGrid() const {
    return m_occupancy_grid;
}

int GridLogic::GetNumberFilledBottomLines() const {
    return m_nr_buttomlines_filled;
}

bool GridLogic::RequestSpaceOnGrid(TetrominoPositionType current_position,
                                   TetrominoPositionType target_position) {
    // TODO(Eugen): Assert current_position.size == target_position.size
    bool is_every_coordinate_within_bounds{true};
    for (const auto &position : target_position) {
        int pos_x{position.first}, pos_y{position.second};
        if (pos_x < 0 || pos_x >= m_number_rows || pos_y < 0 ||
            pos_y >= m_number_columns) {
            is_every_coordinate_within_bounds = false;
            break;
        }
    }

    bool is_request_successfull{false};
    if (is_every_coordinate_within_bounds) {
        try {
            // Check free positions in the desired direction
            bool is_occupied{false};

            // First, identify target cells which do not overlap with the
            // current position's cells
            TetrominoPositionType disjoint_target_cells;
            for (const auto &target_cell : target_position) {
                bool is_target_disjoint{true};
                for (const auto &current_cell : current_position) {
                    is_target_disjoint &= (current_cell != target_cell);
                }
                if (is_target_disjoint) {
                    disjoint_target_cells.push_back(target_cell);
                }
            }

            // In case of no overlapping between current and target position
            // because
            //  - either tetromino was placed initially or
            //  - it was placed at the same place it has been before or
            //  - it was moved so far such that there is no overlap
            // check whether the target position is occupied.
            if (disjoint_target_cells.empty()) {
                for (const auto &cell : target_position) {
                    int row{cell.first}, column{cell.second};
                    is_occupied |= m_occupancy_grid.at(row).at(column);
                }
            }
            // In case of overlapping between current and target position
            // check only for disjoint cells whether they are all occupied.
            else {
                for (const auto &cell : disjoint_target_cells) {
                    int row{cell.first}, column{cell.second};
                    is_occupied |= m_occupancy_grid.at(row).at(column);
                }
            }

            if (!is_occupied) {
                // Free previously occupied positions
                for (const auto &current_cell : current_position) {
                    int current_cell_row{current_cell.first};
                    int current_cell_column{current_cell.second};
                    m_occupancy_grid.at(current_cell_row)
                        .at(current_cell_column) = false;
                }

                // Occupy new positions
                for (const auto &target_cell : target_position) {
                    int target_cell_row{target_cell.first};
                    int target_cell_column{target_cell.second};
                    m_occupancy_grid.at(target_cell_row)
                        .at(target_cell_column) = true;
                }

                is_request_successfull = true;
            }
        } catch (std::out_of_range const &e) {
            std::cerr << e.what() << '\n';
        }
    }

    return is_request_successfull;
}