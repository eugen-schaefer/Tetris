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

std::vector<int> GridLogic::GetIndexesOfFullyOccupiedRows() const {
    return m_indexes_of_fully_occupied_rows;
}

bool GridLogic::RequestSpaceOnGrid(TetrominoPositionType current_position,
                                   TetrominoPositionType target_position) {
    if (current_position.empty() || target_position.empty()) {
        return false;
    }

    if (current_position.size() != target_position.size()) {
        return false;
    }

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

                // put indexes of fully occupied rows in grid
                // from bottom to top into m_indexes_of_fully_occupied_rows
                int row_idx{m_number_rows - 1};
                m_indexes_of_fully_occupied_rows.clear();
                for (auto it = m_occupancy_grid.rbegin();
                     it != m_occupancy_grid.rend(); ++it) {
                    bool is_entire_row_occupied{true};
                    for (const bool is_cell_occupied : (*it)) {
                        is_entire_row_occupied &= is_cell_occupied;
                    }
                    if (is_entire_row_occupied) {
                        m_indexes_of_fully_occupied_rows.push_back(row_idx);
                    }
                    --row_idx;
                    if (row_idx < 0) {
                        break;
                    }
                }

                is_request_successfull = true;
            }
        } catch (std::out_of_range const &e) {
            std::cerr << e.what() << '\n';
        }
    }

    return is_request_successfull;
}

void GridLogic::FreeAllEntirelyOccupiedRows() {
    for (int row_index : m_indexes_of_fully_occupied_rows) {
        // Free entire row
        auto &column = m_occupancy_grid.at(row_index);
        for (auto it = column.begin(); it != column.end();) {
            *it = false;
            ++it;
        }
    }
    m_indexes_of_fully_occupied_rows.clear();
}

void GridLogic::FreeEntireGrid(){
    for (auto &row : m_occupancy_grid){
        std::fill(row.begin(), row.end(), false);
    }
}