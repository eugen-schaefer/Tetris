#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "Game.h"

/// This class controls the entire game. It retrieves the keyboard events and
/// forwards them to the game class but also triggers periodic drops of the
/// active shape. In the end, this class triggers the rendering of all graphics.
class Controller {
   public:
    /// Creates an instance of the Tetris game and center the game window on the
    /// screen.
    /// \param window: Reference to a window which serves as a target for
    ///                2D drawing.
    /// \param font:   Font for all sf::Text instances in the entire
    ///                application.
    Controller(sf::RenderWindow& window, sf::Font& font);

    /// Starts the Tetris game.
    /// \param
    void StartGame(sf::RenderWindow&);

   private:
    int m_number_rows{20};
    int m_number_columns{10};
    Game m_game;
};

#endif /* CONTROLLER_H_ */