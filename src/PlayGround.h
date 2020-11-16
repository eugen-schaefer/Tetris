#ifndef PLAY_GROUND_H_
#define PLAY_GROUND_H_

#include <deque>
#include <memory>
#include <vector>

#include "Dashboard.h"
#include "GridGraphic.h"
#include "GridLogic.h"
#include "TetrominoGraphic.h"

class PlayGround : public sf::Drawable {
   public:
    PlayGround(int number_grid_rows, int number_grid_columns,
               const sf::RenderWindow& window, sf::Font& font);
    void ProcessKeyEvents(sf::Event event);
    void ProcessLockDown();
    void MoveActiveShapeOneStepDown();
    void StartNewGame();
    bool IsGameOver() { return m_is_game_over; };
    bool IsGameOverAlreadyAnnounced() { return m_is_game_over_announced; };
    void SetGameOverAnnounced() { m_is_game_over_announced = true; };

   private:
    int m_number_grid_rows, m_number_grid_columns;
    bool m_is_game_over, m_is_game_over_announced;
    sf::Text m_game_over_text;
    sf::Text m_start_new_game_text;
    GridLogic m_grid_logic;
    GridGraphic m_grid_graphic;
    std::deque<std::unique_ptr<TetrominoGraphic>> m_shapes_in_queue;
    std::vector<std::unique_ptr<TetrominoGraphic>> m_frozen_shapes_on_grid;
    std::unique_ptr<TetrominoGraphic> m_active_shape;
    Dashboard m_dashboard;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif /* PLAY_GROUND_H_ */