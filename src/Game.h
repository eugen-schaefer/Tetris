#ifndef GAME_H_
#define GAME_H_

#include "PlayGround.h"

class Game {
   public:
    Game(sf::RenderWindow&, sf::Font& font);
    void start(sf::RenderWindow&);

   private:
    int m_number_rows{20};
    int m_number_columns{10};
    PlayGround m_play_ground;
};

#endif /* GAME_H_ */