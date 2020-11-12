#include "PlayGround.h"

#include <SFML/Graphics.hpp>

class ShapeFactory {
   public:
    static std::unique_ptr<TetrominoGraphic> create(GridLogic& grid_logic,
                                                    GridGraphic& grid_graphic) {
        // TODO(Eugen): workarround for the start, consider random generation
        // here
        std::unique_ptr<TetrominoGraphic> m_active_graphic_shape_ptr =
            std::make_unique<TetrominoGraphic>(
                std::make_unique<ShapeI>(grid_logic), grid_graphic);
        return std::move(m_active_graphic_shape_ptr);
    }
};

PlayGround::PlayGround(int number_grid_rows, int number_grid_columns,
                       const sf::RenderWindow& window)
    : m_grid_logic{GridLogic(number_grid_rows, number_grid_columns)},
      m_grid_graphic{GridGraphic(number_grid_rows, number_grid_columns, 0.1f,
                                 window.getSize())} {
    // generate two random shapes
    m_shapes_in_queue.push_front(
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));
    m_shapes_in_queue.push_front(
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));

    m_active_shape =
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic));
}

void PlayGround::Update(sf::Event event) {
    if (m_active_shape) {
        // process keyboard event for the active shape
        if ((event.type == sf::Event::KeyPressed) &&
            (event.key.code == sf::Keyboard::Left)) {
            m_active_shape->MoveOneStep(Direction::left);
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Right)) {
            m_active_shape->MoveOneStep(Direction::right);
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Down)) {
            m_active_shape->MoveOneStep(Direction::down);
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Up)) {
            m_active_shape->Rotate();
        }

        // In case the active shape is frozen because it reached the lowest
        // possible level on the grid, put it into m_frozen_shapes_on_grid. Then
        // generate a new shape and put it in front of m_shapes_in_queue and pop
        // a shape from the back of the m_shapes_in_queue. Finally, check
        // occupancy grid for entirely occupied rows and clear them all if any.
        if (!m_active_shape->IsMovable()) {
            m_frozen_shapes_on_grid.push_back(std::move(m_active_shape));
            m_active_shape = std::move(m_shapes_in_queue.back());
            m_shapes_in_queue.pop_back();
            m_shapes_in_queue.push_front(
                std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));

            // check the occupancy grid for fully occupied rows
            std::vector<int> vector_of_indexes_of_fully_occupied_rows =
                m_grid_logic.GetIndexesOfFullyOccupiedRows();

            // Clear entirely occupied rows
            if (!vector_of_indexes_of_fully_occupied_rows.empty()) {
                // Identify all affected tetrominoes by the fully occupied row
                // and remove their affected parts (squares)
                for (const int occupied_row_index :
                     vector_of_indexes_of_fully_occupied_rows) {
                    for (auto graphical_shape_iterator =
                             m_frozen_shapes_on_grid.begin();
                         graphical_shape_iterator !=
                         m_frozen_shapes_on_grid.end();) {
                        auto logical_chape_iterator =
                            (*graphical_shape_iterator)
                                ->GetIteratorToBeginOfLogicalTetromino();
                        for (auto graphical_iterator =
                                 (*graphical_shape_iterator)
                                     ->GetIteratorToBeginOfGraphicalTetromino();
                             graphical_iterator !=
                             (*graphical_shape_iterator)
                                 ->GetIteratorToEndOfGraphicalTetromino();) {
                            if (logical_chape_iterator->first == occupied_row_index) {
                                (*graphical_shape_iterator)
                                    ->DeleteTetrominoSquare(graphical_iterator,
                                                            logical_chape_iterator);
                            } else {
                                ++graphical_iterator;
                                ++logical_chape_iterator;
                            }
                        }
                        if ((*graphical_shape_iterator)->GetSquares().empty()) {
                            graphical_shape_iterator =
                                m_frozen_shapes_on_grid.erase(
                                    graphical_shape_iterator);
                        } else {
                            ++graphical_shape_iterator;
                        }
                    }
                }

                // Free released rows
                m_grid_logic.FreeAllEntirelyOccupiedRows();

                // Unlock the movability of each tetromino on the grid, move
                // each one down until it hits an obstacle and lock it again
                for (auto& shape : m_frozen_shapes_on_grid) {
                    shape->MakeMovable();
                    while (shape->MoveOneStep(Direction::down)) {
                    }
                    shape->MakeUnmovable();
                }
            }
        }
    }
}

void PlayGround::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_grid_graphic, states);
    if (m_active_shape) {
        target.draw(*m_active_shape, states);
    }
    if (!m_frozen_shapes_on_grid.empty()) {
        for (const auto& graphical_shape : m_frozen_shapes_on_grid) {
            target.draw(*graphical_shape);
        }
    }
}
