#ifndef GAME_H_
#define GAME_H_

#include <deque>
#include <memory>
#include <vector>

#include "Dashboard.h"
#include "GridGraphic.h"
#include "GridLogic.h"
#include "TetrominoGraphic.h"

/// The Game class provides all necessities to start a game. Concrete, it
/// constructs the Tetris grid, a dashboard, and all tetrominoes falling from
/// top to bottom. Tetrominoes are organized in the following way: Once a
/// tetromino shape is randomly generated, it is pushed into a waiting queue
/// containing three tetrominoes in total. Then one tetromino is poped from
/// another end of the queue and referred to as an active shape. The player can
/// relocate and rotate this active shape as long as it is not locked down. When
/// the active shape has reached the lowest possible free line on the grid, it
/// is then locked down and pushed into another container that contains all the
/// locked shapes. Furthermore, the Game class offers methods to process
/// keyboard events, shapes lock down and to restart the game.
class Game : public sf::Drawable {
   public:
    /// Creates a drawable grid object, a drawable dashboard, sets up the
    /// game over / new game text message, and starts a new game.
    /// \param number_grid_rows: Number of rows in the game grid.
    /// \param number_grid_columns: Number of columns in the game grid.
    /// \param window: Reference to a window which serves as a target
    ///                for 2D drawing.
    /// \param font: Font for all sf::Text instances in the entire application.
    Game(int number_grid_rows, int number_grid_columns,
         const sf::RenderWindow& window, sf::Font& font);

    /// Processes an event from the keyboard
    /// \param event: event being processed
    void ProcessKeyEvent(sf::Event event);

    /// Processes a lock-down-state. A lock-down-state is achieved when the next
    /// tetromino shape cannot be placed on the grid anymore because the grid is
    /// already full of shapes.
    void ProcessLockDown();

    /// Moves the active shape one step downwards. The method sets also the
    /// corresponding flag when the game is over.
    void MoveActiveShapeOneStepDown();

    /// Starts a new game by resetting all current states like scoring, list of
    /// locked shapes etc.
    void StartNewGame();

    /// Retrieves the information whether the game is over or not.
    bool IsGameOver() { return m_is_game_over; };

    /// In case the game is over, retrieves the information about whether this
    /// fact has been already announced. This information is then utilized in
    /// the controller class to draw the very last game state only once after
    /// the game window has been cleared in the controller cycle.
    bool IsGameOverAlreadyAnnounced() { return m_is_game_over_announced; };

    /// Sets the game-over-announcement flag to true.
    void SetGameOverAnnounced() { m_is_game_over_announced = true; };

   private:
    int m_number_grid_rows, m_number_grid_columns;
    bool m_is_game_over, m_is_game_over_announced;
    sf::Text m_game_over_text;
    sf::Text m_start_new_game_text;
    GridLogic m_grid_logic;
    GridGraphic m_grid_graphic;
    std::deque<std::unique_ptr<TetrominoGraphic>> m_shapes_in_queue;
    std::vector<std::unique_ptr<TetrominoGraphic>> m_locked_shapes_on_grid;
    std::unique_ptr<TetrominoGraphic> m_active_shape;
    Dashboard m_dashboard;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif /* GAME_H_ */