#include "Dashboard.h"

void set_origin_to_middle(sf::Text &text) {
    auto label_width = text.getLocalBounds().width;
    auto label_height = text.getGlobalBounds().height;
    text.setOrigin(label_width / 2.f, label_height / 2.f);
}

Dashboard::Dashboard(float offset_window_top_border, float max_available_width,
                     float max_available_height, sf::Font &font)
    : m_score{0}, m_number_cleared_lines{0} {
    // general dashboard appearance details
    constexpr unsigned int kOutlineThickness{2};
    constexpr unsigned int kLabelCharacterSize{30};
    constexpr unsigned int kNumberSize{50};
    const sf::Color kLabelColor{sf::Color::Blue};
    const sf::Color kNumberColor{sf::Color::Blue};
    const sf::Color kQueueBorderColor{sf::Color::Red};
    const sf::Color kScoringBorderColor{sf::Color::Red};

    // dashboard related details
    float relative_width_utilization{0.6};
    float dashboard_width{relative_width_utilization * max_available_width};
    float distance_between_queue_rect_bottom_edge_and_scoring_top_edge{
        0.1f * max_available_height};

    // geometric details for the queue rectangle
    float queue_rect_height{0.5f * max_available_height};
    float pos_x_top_left_corner_queue_rect{
        max_available_width * (1 - relative_width_utilization) / 2.f};
    float pos_y_top_left_corner_queue_rect{offset_window_top_border};

    // geometric details for the scoring rectangle
    float scoring_rect_height{0.55f * queue_rect_height};
    float pos_x_top_left_corner_scoring_rect{pos_x_top_left_corner_queue_rect};
    float pos_y_top_left_corner_scoring_rect{
        offset_window_top_border + queue_rect_height +
        distance_between_queue_rect_bottom_edge_and_scoring_top_edge};

    // create queue related information
    m_queue_border =
        sf::RectangleShape(sf::Vector2f(dashboard_width, queue_rect_height));
    m_queue_border.setPosition(pos_x_top_left_corner_queue_rect,
                               pos_y_top_left_corner_queue_rect);
    m_queue_border.setOutlineThickness(kOutlineThickness);
    m_queue_border.setOutlineColor(kQueueBorderColor);

    m_queue_label.setString("Next");
    m_queue_label.setFillColor(kLabelColor);
    m_queue_label.setFont(font);
    m_queue_label.setCharacterSize(kLabelCharacterSize);
    m_queue_label.setPosition(
        pos_x_top_left_corner_queue_rect + dashboard_width / 2.f,
        pos_y_top_left_corner_queue_rect);
    set_origin_to_middle(m_queue_label);
    m_queue_label.setStyle(sf::Text::Underlined);

    // create a grid which supports placing the
    // queue element on the dashboard
    float grid_height{0.8f * queue_rect_height};
    float grid_cell_side_length =
        grid_height / static_cast<float>(m_number_grid_rows);
    float pos_x_top_left_corner_grid{m_queue_label.getPosition().x -
                                     2.f * grid_cell_side_length};
    float pos_y_top_left_corner_grid{
        m_queue_label.getGlobalBounds().top +
        2.f * m_queue_label.getGlobalBounds().height};
    m_dashboard_grid_graphic = GridGraphic(
        m_number_grid_rows, m_number_grid_columns, pos_x_top_left_corner_grid,
        pos_y_top_left_corner_grid, grid_height);

    // create scoring related information
    m_scoring_border =
        sf::RectangleShape(sf::Vector2f(dashboard_width, scoring_rect_height));
    m_scoring_border.setPosition(pos_x_top_left_corner_scoring_rect,
                                 pos_y_top_left_corner_scoring_rect);
    m_scoring_border.setOutlineThickness(kOutlineThickness);
    m_scoring_border.setOutlineColor(kScoringBorderColor);

    m_score_label.setString("Score");
    m_score_label.setFillColor(kLabelColor);
    m_score_label.setFont(font);
    m_score_label.setCharacterSize(kLabelCharacterSize);
    m_score_label.setPosition(
        pos_x_top_left_corner_scoring_rect + dashboard_width / 2.f,
        pos_y_top_left_corner_scoring_rect);
    set_origin_to_middle(m_score_label);
    m_score_label.setStyle(sf::Text::Underlined);

    m_score_number.setString("0");
    m_score_number.setFillColor(kNumberColor);
    m_score_number.setFont(font);
    m_score_number.setCharacterSize(kNumberSize);
    m_score_number.setPosition(
        pos_x_top_left_corner_scoring_rect + dashboard_width / 2.f,
        m_score_label.getGlobalBounds().top +
            2.f * m_score_label.getLocalBounds().height);
    set_origin_to_middle(m_score_number);

    m_cleared_lines_label1.setString("Cleared");
    m_cleared_lines_label1.setFillColor(kLabelColor);
    m_cleared_lines_label1.setFont(font);
    m_cleared_lines_label1.setCharacterSize(kLabelCharacterSize);
    m_cleared_lines_label1.setPosition(
        pos_x_top_left_corner_scoring_rect + dashboard_width / 2.f,
        m_score_number.getGlobalBounds().top +
            2.3f * m_score_number.getLocalBounds().height);
    set_origin_to_middle(m_cleared_lines_label1);
    m_cleared_lines_label1.setStyle(sf::Text::Underlined);

    m_cleared_lines_label2.setString("lines");
    m_cleared_lines_label2.setFillColor(kLabelColor);
    m_cleared_lines_label2.setFont(font);
    m_cleared_lines_label2.setCharacterSize(kLabelCharacterSize);
    m_cleared_lines_label2.setPosition(
        pos_x_top_left_corner_scoring_rect + dashboard_width / 2.f,
        m_cleared_lines_label1.getGlobalBounds().top +
            1.f * m_cleared_lines_label1.getLocalBounds().height);
    set_origin_to_middle(m_cleared_lines_label2);
    m_cleared_lines_label2.setStyle(sf::Text::Underlined);

    m_cleared_lines_number.setString("0");
    m_cleared_lines_number.setFillColor(kNumberColor);
    m_cleared_lines_number.setFont(font);
    m_cleared_lines_number.setCharacterSize(kNumberSize);
    m_cleared_lines_number.setPosition(
        pos_x_top_left_corner_scoring_rect + dashboard_width / 2.f,
        m_cleared_lines_label2.getGlobalBounds().top +
            2.f * m_cleared_lines_label2.getLocalBounds().height);
    set_origin_to_middle(m_cleared_lines_number);
}

void Dashboard::InsertNextTetromino(TetrominoType shape) {
    // delete first element in queue
    if (m_shapes_in_queue.size() > 2) {
        m_shapes_in_queue.pop_front();
    }

    // move remaining elements on dashboard grid 4 steps up
    for (size_t i{0}; i < m_shapes_in_queue.size(); i++) {
        TetrominoPositionType pos{
            m_shapes_in_queue.at(i).GetPositionInGridLogicFrame()};
        for (auto &elem : pos) {
            elem.first -= 5;
        }
        m_shapes_in_queue.at(i).SetPositionInDashboard(pos);
    }

    // Insert a new element to the back of the queue and
    // place it as the lowest element on the dashboard grid
    TetrominoPositionType init_position{};
    switch (shape) {
        case TetrominoType::I:
            init_position = {{12, 0}, {12, 1}, {12, 2}, {12, 3}};
            m_shapes_in_queue.emplace_back(
                std::make_unique<ShapeI>(m_dashboard_grid_logic, init_position),
                m_dashboard_grid_graphic);
            break;
        case TetrominoType::J:
            init_position = {{11, 0}, {12, 0}, {12, 1}, {12, 2}};
            m_shapes_in_queue.emplace_back(
                std::make_unique<ShapeJ>(m_dashboard_grid_logic, init_position),
                m_dashboard_grid_graphic);
            break;
    }
};

void Dashboard::AddToScore(int score) {
    m_score += score;
    m_score_number.setString(std::to_string(m_score));
    set_origin_to_middle(m_score_number);
}

void Dashboard::AddToClearedLines(int nr_cleared_lines) {
    m_number_cleared_lines += nr_cleared_lines;
    m_cleared_lines_number.setString(std::to_string(m_number_cleared_lines));
    set_origin_to_middle(m_cleared_lines_number);
}

void Dashboard::Reset() {
    m_shapes_in_queue.clear();
    m_score = 0;
    m_number_cleared_lines = 0;
    m_score_number.setString(std::to_string(m_score));
    set_origin_to_middle(m_score_number);
    m_cleared_lines_number.setString(std::to_string(m_number_cleared_lines));
    set_origin_to_middle(m_cleared_lines_number);
}

void Dashboard::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_queue_border, states);
    target.draw(m_scoring_border, states);
    target.draw(m_queue_label, states);
    target.draw(m_score_label, states);
    target.draw(m_score_number, states);
    target.draw(m_cleared_lines_label1, states);
    target.draw(m_cleared_lines_label2, states);
    target.draw(m_cleared_lines_number, states);
    target.draw(m_dashboard_grid_graphic, states);

    for (auto &elem : m_shapes_in_queue) {
        target.draw(elem, states);
    }
}
