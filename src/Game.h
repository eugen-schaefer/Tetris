#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>

#include "GridGraphic.h"

class Game {
   public:
    Game(sf::RenderWindow&);
    void start(sf::RenderWindow&);

   private:
    GridGraphic m_grid;
};

#endif /* GAME_H_ */