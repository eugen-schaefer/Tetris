#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "PlayGround.h"

class Controller {
   public:
    Controller(sf::RenderWindow&, sf::Font& font);
    void StartGame(sf::RenderWindow&);

   private:
    int m_number_rows{20};
    int m_number_columns{10};
    PlayGround m_play_ground;
};

#endif /* CONTROLLER_H_ */