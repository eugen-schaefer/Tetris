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
    void Update(sf::Event event);

   private:
    GridLogic m_grid_logic;
    GridGraphic m_grid_graphic;
    std::deque<std::unique_ptr<TetrominoGraphic>> m_shapes_in_queue;
    std::vector<std::unique_ptr<TetrominoGraphic>> m_frozen_shapes_on_grid;
    std::unique_ptr<TetrominoGraphic> m_active_shape;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    Dashboard m_dashboard;
};

#endif /* PLAY_GROUND_H_ */