#include "PlayGround.h"

#include <SFML/Graphics.hpp>
#include <algorithm>

class ShapeFactory {
   public:
    static std::unique_ptr<TetrominoGraphic> create(GridLogic& grid_logic,
                                                    GridGraphic& grid_graphic) {
        // TODO(Eugen): workarround for the start, consider random generation
        // here
        // TetrominoPositionType init_position = {{0, 3}, {0, 4}, {0, 5}, {0, 6}};  //ShapeI
        // TetrominoPositionType init_position = {{0, 3}, {1, 3}, {1, 4}, {1, 5}};  //ShapeJ
        // TetrominoPositionType init_position = {{0, 5}, {1, 3}, {1, 4}, {1, 5}};  // ShapeL
        // TetrominoPositionType init_position = {{0, 4}, {0, 5}, {1, 5}, {1, 4}};  // ShapeO
        // TetrominoPositionType init_position = {{0, 4}, {0, 5}, {1, 4}, {1, 3}};  // ShapeS
        TetrominoPositionType init_position = {{0, 4}, {1, 3}, {1, 4}, {1, 5}};  // ShapeT
        std::unique_ptr<TetrominoGraphic> m_active_graphic_shape_ptr =
            std::make_unique<TetrominoGraphic>(
                std::make_unique<ShapeT>(grid_logic, init_position),
                grid_graphic);
        return std::move(m_active_graphic_shape_ptr);
    }
};

PlayGround::PlayGround(int number_grid_rows, int number_grid_columns,
                       const sf::RenderWindow& window, sf::Font& font)
    : m_number_grid_rows{number_grid_rows},
      m_number_grid_columns{number_grid_columns},
      m_is_game_over{false},
      m_is_game_over_announced{false},
      m_grid_logic{GridLogic(number_grid_rows, number_grid_columns)} {
    // Create a drawble grid object
    float relative_top_margin{0.1f};
    float window_width{static_cast<float>(window.getSize().x)};
    float window_height{static_cast<float>(window.getSize().y)};

    float grid_height{(1.0f - 2.0f * relative_top_margin) * window_height};
    float grid_cell_side_length =
        grid_height / static_cast<float>(number_grid_rows);
    float grid_width{static_cast<float>(number_grid_columns) *
                     grid_cell_side_length};

    float grid_pos_x_top_left_corner{
        (1.0f - relative_top_margin) * window_width - grid_width};
    float grid_pos_y_top_left_corner{relative_top_margin * window_height};

    m_grid_graphic = GridGraphic(number_grid_rows, number_grid_columns,
                                 grid_pos_x_top_left_corner,
                                 grid_pos_y_top_left_corner, grid_height);

    // generate a dashboard
    float offset_window_top_border{0.1f *
                                   static_cast<float>(window.getSize().y)};
    float available_width{window.getSize().x - grid_width -
                          0.1f * window.getSize().x};
    float max_available_height{grid_height};
    m_dashboard = Dashboard(offset_window_top_border, available_width,
                            max_available_height, font);

    // Setup game over text
    m_game_over_text.setString("!!! GAME OVER !!!");
    m_game_over_text.setFillColor(sf::Color::Red);
    m_game_over_text.setFont(font);
    m_game_over_text.setCharacterSize(60);
    m_game_over_text.setStyle(sf::Text::Bold);
    auto grid_position_top_left_corner =
        m_grid_graphic.GetPositionRelativeToWindow(0, 0);
    auto grid_position_top_right_corner =
        m_grid_graphic.GetPositionRelativeToWindow(0,
                                                   m_number_grid_columns - 1);
    auto grid_position_bottom_left_corner =
        m_grid_graphic.GetPositionRelativeToWindow(m_number_grid_rows - 1, 0);

    float text_position_x{grid_pos_x_top_left_corner + grid_width / 2.f};
    float text_position_y{grid_pos_y_top_left_corner + grid_height / 3.f};

    m_game_over_text.setPosition(text_position_x, text_position_y);

    auto label_width = m_game_over_text.getLocalBounds().width;
    auto label_height = m_game_over_text.getGlobalBounds().height;
    m_game_over_text.setOrigin(label_width / 2.f, label_height / 2.f);

    // Setup new game text
    m_start_new_game_text.setString("Start new game with Ctrl + N");
    m_start_new_game_text.setFillColor(sf::Color::Blue);
    m_start_new_game_text.setFont(font);
    m_start_new_game_text.setCharacterSize(34);
    m_start_new_game_text.setStyle(sf::Text::Bold);
    text_position_x = grid_pos_x_top_left_corner + grid_width / 2.f;
    text_position_y = m_game_over_text.getGlobalBounds().top +
                      2.f * m_game_over_text.getGlobalBounds().height;
    m_start_new_game_text.setPosition(text_position_x, text_position_y);
    label_width = m_start_new_game_text.getLocalBounds().width;
    label_height = m_start_new_game_text.getGlobalBounds().height;
    m_start_new_game_text.setOrigin(label_width / 2.f, label_height / 2.f);

    StartNewGame();
}

void PlayGround::ProcessKeyEvents(sf::Event event) {
    if (m_active_shape) {
        // process keyboard event for the active shape
        if ((event.type == sf::Event::KeyPressed) &&
            (event.key.code == sf::Keyboard::Left) && (!m_is_game_over)) {
            m_active_shape->MoveOneStep(Direction::left);
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Right) &&
                   (!m_is_game_over)) {
            m_active_shape->MoveOneStep(Direction::right);
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Down) &&
                   (!m_is_game_over)) {
            bool is_movement_succeed{
                m_active_shape->MoveOneStep(Direction::down)};
            if (!is_movement_succeed &&
                (m_active_shape->GetHighestRow() == 0)) {
                m_is_game_over = true;
            }
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Up) && (!m_is_game_over)) {
            m_active_shape->Rotate();
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.control) && (event.key.code == sf::Keyboard::N)) {
            StartNewGame();
        }
    }
}

void PlayGround::ProcessLockDown() {
    if (m_active_shape) {
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
            m_dashboard.InsertNextTetromino(
                m_shapes_in_queue.at(0)->GetTetrominoType());

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
                            if (logical_chape_iterator->first ==
                                occupied_row_index) {
                                (*graphical_shape_iterator)
                                    ->DeleteTetrominoSquare(
                                        graphical_iterator,
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

                // update the score
                // use the original BPS scoring system, see
                // https://tetris.wiki/Scoring#Recent_guideline_compatible_games)
                std::sort(vector_of_indexes_of_fully_occupied_rows.begin(),
                          vector_of_indexes_of_fully_occupied_rows.end());

                // First, identify single, double, triple or tetris
                std::vector<int> line_clears;
                int nr_successively_cleared_lines{1};
                int prev_index{-10};
                int total_number_cleared_lines{static_cast<int>(
                    vector_of_indexes_of_fully_occupied_rows.size())};
                for (size_t i = 0; i < total_number_cleared_lines; i++) {
                    int row_index{
                        vector_of_indexes_of_fully_occupied_rows.at(i)};
                    bool is_first_loop_iteration{
                        (i == 0) && (total_number_cleared_lines > 1)};
                    if (is_first_loop_iteration) {
                        prev_index = row_index;
                        continue;
                    }
                    bool is_prev_line_cleared{(row_index - prev_index) == 1};
                    if (is_prev_line_cleared) {
                        ++nr_successively_cleared_lines;
                        bool is_not_last_loop_iteration{
                            i < total_number_cleared_lines - 1};
                        if (is_not_last_loop_iteration) {
                            prev_index = row_index;
                            continue;
                        }
                    }
                    line_clears.push_back(nr_successively_cleared_lines);
                    nr_successively_cleared_lines = 1;
                    prev_index = row_index;
                }

                // Then, calculate the corresponding score
                for (int line_clear : line_clears) {
                    switch (line_clear) {
                        case 1:
                            m_dashboard.AddToScore(40);
                            m_dashboard.AddToClearedLines(1);
                            break;
                        case 2:
                            m_dashboard.AddToScore(100);
                            m_dashboard.AddToClearedLines(2);
                            break;
                        case 3:
                            m_dashboard.AddToScore(300);
                            m_dashboard.AddToClearedLines(3);
                            break;
                        case 4:
                            m_dashboard.AddToScore(1200);
                            m_dashboard.AddToClearedLines(4);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

void PlayGround::MoveActiveShapeOneStepDown() {
    if (m_active_shape) {
        bool is_movement_succeed{m_active_shape->MoveOneStep(Direction::down)};
        if (!is_movement_succeed && (m_active_shape->GetHighestRow() == 0)) {
            m_is_game_over = true;
        }
    }
}

void PlayGround::StartNewGame() {
    // Reset the state of current game
    m_is_game_over = false;
    m_is_game_over_announced = false;
    m_grid_logic.FreeEntireGrid();
    m_shapes_in_queue.clear();
    m_frozen_shapes_on_grid.clear();
    m_active_shape = nullptr;
    m_dashboard.Reset();

    // generate three random shapes and put them all in a queue
    m_shapes_in_queue.push_front(
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));
    m_shapes_in_queue.push_front(
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));
    m_shapes_in_queue.push_front(
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));

    // generate a shape being actively moved on the grid
    m_active_shape =
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic));

    // insert all elements from m_shapes_in_queue into the dashboard queue
    m_dashboard.InsertNextTetromino(
        m_shapes_in_queue.at(0)->GetTetrominoType());
    m_dashboard.InsertNextTetromino(
        m_shapes_in_queue.at(1)->GetTetrominoType());
    m_dashboard.InsertNextTetromino(
        m_shapes_in_queue.at(2)->GetTetrominoType());
}

void PlayGround::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_is_game_over) {
        target.draw(m_game_over_text, states);
        target.draw(m_start_new_game_text, states);
    } else {
        target.draw(m_grid_graphic, states);
        if (m_active_shape) {
            target.draw(*m_active_shape, states);
        }
        if (!m_frozen_shapes_on_grid.empty()) {
            for (const auto& graphical_shape : m_frozen_shapes_on_grid) {
                target.draw(*graphical_shape);
            }
        }
        target.draw(m_dashboard);
    }
}
