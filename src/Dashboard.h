#ifndef DASHBOARD_H_
#define DASHBOARD_H_

#include <SFML/Graphics.hpp>
#include <deque>

#include "GridLogic.h"
#include "TetrominoGraphic.h"

using TetrominoQueueType = std::deque<std::unique_ptr<TetrominoGraphic>>;

class Dashboard : public sf::Drawable {
   public:
    Dashboard() = default;

    // offset_window_top_border:  distance between the window top edge and the
    //                            top of the dashboard
    // available_width:           maximum width between the window left edge and
    //                            the grid left edge
    Dashboard(float offset_window_top_border, float available_width,
              float max_available_height, sf::Font& font);

    void InsertNextTetromino(TetrominoType shape);

    void AddToScore(int score);

    void AddToClearedLines(int nr_cleared_lines);

   private:
    unsigned int m_score{};
    unsigned int m_number_cleared_lines{};
    sf::RectangleShape m_queue_border;
    sf::RectangleShape m_scoring_border;
    sf::Text m_queue_label;
    sf::Text m_score_label;
    sf::Text m_score_number;
    sf::Text m_cleared_lines_label1;
    sf::Text m_cleared_lines_label2;
    sf::Text m_cleared_lines_number;
    sf::Font m_font;
    GridGraphic m_dashboard_grid_graphic;
    int m_number_grid_rows{14};
    int m_number_grid_columns{4};
    GridLogic m_dashboard_grid_logic{m_number_grid_rows, m_number_grid_columns};
    std::deque<TetrominoGraphic> m_shapes_in_queue{};

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif /* DASHBOARD_H_ */