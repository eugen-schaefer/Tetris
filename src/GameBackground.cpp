#include "GameBackground.h"

#include <iostream>

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
    TetrinosCoordinateType requested_coordinates) {
    auto coordinate_0 = std::get<0>(requested_coordinates);
    auto coordinate_1 = std::get<1>(requested_coordinates);
    auto coordinate_2 = std::get<2>(requested_coordinates);
    auto coordinate_3 = std::get<3>(requested_coordinates);

    bool is_every_coordinate_within_bounds{

        coordinate_0.first >= 0 && coordinate_0.first < m_vertical_grid_size &&
        coordinate_0.second >= 0 &&
        coordinate_0.second < m_horizontal_grid_size &&

        coordinate_1.first >= 0 && coordinate_1.first < m_vertical_grid_size &&
        coordinate_1.second >= 0 &&
        coordinate_1.second < m_horizontal_grid_size &&

        coordinate_2.first >= 0 && coordinate_2.first < m_vertical_grid_size &&
        coordinate_2.second >= 0 &&
        coordinate_2.second < m_horizontal_grid_size &&

        coordinate_3.first >= 0 && coordinate_3.first < m_vertical_grid_size &&
        coordinate_3.second >= 0 && coordinate_3.second < m_horizontal_grid_size

    };

    bool is_request_successfull{false};
    if (is_every_coordinate_within_bounds) {
        try {
            bool is_occupied = (m_occupancy_grid.at(coordinate_0.first)
                                    .at(coordinate_0.second) ||
                                m_occupancy_grid.at(coordinate_1.first)
                                    .at(coordinate_1.second) ||
                                m_occupancy_grid.at(coordinate_2.first)
                                    .at(coordinate_2.second) ||
                                m_occupancy_grid.at(coordinate_3.first)
                                    .at(coordinate_3.second));
            if (!is_occupied) {
                m_occupancy_grid.at(coordinate_0.first)
                    .at(coordinate_0.second) = true;
                m_occupancy_grid.at(coordinate_1.first)
                    .at(coordinate_1.second) = true;
                m_occupancy_grid.at(coordinate_2.first)
                    .at(coordinate_2.second) = true;
                m_occupancy_grid.at(coordinate_3.first)
                    .at(coordinate_3.second) = true;
                is_request_successfull = true;
            }
        } catch (std::out_of_range const& e) {
            std::cerr << e.what() << '\n';
        }
    }

    return is_request_successfull;
}
