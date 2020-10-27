#include "GameBackground.h"

GameBackground::GameBackground(int vertical_grid_size, int horizontal_grid_size)
    : m_horizontal_grid_size{horizontal_grid_size},
      m_vertical_grid_size{vertical_grid_size} {
    m_occupancy_grid.resize(vertical_grid_size);
    for (auto& row : m_occupancy_grid) {
        row.resize(horizontal_grid_size);
    }
}

std::vector<std::vector<bool>> GameBackground::GetOccupancyGrid() const {
    return m_occupancy_grid;
}
int GameBackground::GetNumberFilledBottomLines() const {
    return m_nr_buttomlines_filled;
}

bool GameBackground::RequestSpaceOnGrid(
    TetrominoPositionType requested_coordinates) {
    bool is_every_coordinate_within_bounds{true};
    for (const auto& position : requested_coordinates) {
        int pos_x{position.first}, pos_y{position.second};
        if (pos_x < 0 || pos_x >= m_vertical_grid_size || pos_y < 0 ||
            pos_y >= m_horizontal_grid_size) {
            is_every_coordinate_within_bounds = false;
            break;
        }
    }

    bool is_request_successfull{false};
    if (is_every_coordinate_within_bounds) {
        try {
            bool is_occupied{false};
            for (const auto& position : requested_coordinates) {
                int row{position.first}, column{position.second};
                is_occupied |= m_occupancy_grid.at(row).at(column);
            }

            if (!is_occupied) {
                for (const auto& position : requested_coordinates) {
                    int row{position.first}, column{position.second};
                    m_occupancy_grid.at(row).at(column) = true;
                }
                is_request_successfull = true;
            }
        } catch (std::out_of_range const& e) {
            std::cerr << e.what() << '\n';
        }
    }
    
    return is_request_successfull;
}