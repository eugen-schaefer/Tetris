#include "TetrominoGraphic.h"

#include "GridLogic.h"

TetrominoGraphic::TetrominoGraphic(std::unique_ptr<Tetromino> shape,
                                   const GridGraphic& grid_graphic)
    : m_shape{std::move(shape)}, m_grid_graphic{grid_graphic} {
    sf::Color tetromino_color{sf::Color::Black};

    // map shape-color to sf-color
    switch (m_shape->GetColor()) {
        case Color::blue:
            tetromino_color = sf::Color::Blue;
            break;
        case Color::cyan:
            tetromino_color = sf::Color::Cyan;
            break;
        case Color::green:
            tetromino_color = sf::Color::Green;
            break;
        case Color::orange:
            tetromino_color = sf::Color(255, 165, 0);
            break;
        case Color::purple:
            tetromino_color = sf::Color(128, 0, 128);
            break;
        case Color::red:
            tetromino_color = sf::Color::Red;
            break;
        case Color::yellow:
            tetromino_color = sf::Color::Yellow;
            break;
        default:
            break;
    }

    // instantiate 4 squares representing the tetromino shape on the GridGraphic
    // with init values like position at x=0, y=0
    for (int index{0}; index < m_squares.size(); ++index) {
        m_squares.at(index) = sf::RectangleShape(
            sf::Vector2f(grid_graphic.GetGridCellSideLength(),
                         grid_graphic.GetGridCellSideLength()));
        m_squares.at(index).setFillColor(tetromino_color);
    }

    UpdatePosition();
}

void TetrominoGraphic::MoveOneStep(Direction direction) {
    m_shape->MoveOneStep(direction);
    UpdatePosition();
}

void TetrominoGraphic::Rotate() {
    m_shape->Rotate();
    UpdatePosition();
}

void TetrominoGraphic::UpdatePosition() {
    auto tetromino_position = m_shape->GetPosition();
    for (int index{0}; index < m_squares.size(); ++index) {
        int rowindex_in_grid = tetromino_position.at(index).first;
        int columnindex_in_grid = tetromino_position.at(index).second;
        sf::Vector2f relative_position =
            *m_grid_graphic.GetPositionRelativeToWindow(rowindex_in_grid,
                                                        columnindex_in_grid);
        m_squares.at(index).setPosition(relative_position);
    }
}

void TetrominoGraphic::draw(sf::RenderTarget& target,
                            sf::RenderStates states) const {
    for (auto& square : m_squares) {
        target.draw(square, states);
    }
}