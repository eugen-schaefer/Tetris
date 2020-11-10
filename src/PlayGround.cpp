#include "PlayGround.h"

#include <SFML/Graphics.hpp>

class ShapeFactory {
   public:
    static std::unique_ptr<TetrominoGraphic> create(GridLogic& grid_logic,
                                                    GridGraphic& grid_graphic) {
        // TODO(Eugen): workarround for the start, consider random generation
        // here
        std::unique_ptr<TetrominoGraphic> m_active_graphic_shape_ptr =
            std::make_unique<TetrominoGraphic>(
                std::make_unique<ShapeI>(grid_logic), grid_graphic);
        return std::move(m_active_graphic_shape_ptr);
    }
};

PlayGround::PlayGround(int number_grid_rows, int number_grid_columns,
                       const sf::RenderWindow& window)
    : m_grid_logic{GridLogic(number_grid_rows, number_grid_columns)},
      m_grid_graphic{GridGraphic(number_grid_rows, number_grid_columns, 0.1f,
                                 window.getSize())} {
    // generate two random shapes
    m_shapes_in_queue.push_front(
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));
    m_shapes_in_queue.push_front(
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));

    m_active_shape =
        std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic));
}

void PlayGround::Update(sf::Event event) {
    if (m_active_shape) {
        if ((event.type == sf::Event::KeyPressed) &&
            (event.key.code == sf::Keyboard::Left)) {
            m_active_shape->MoveOneStep(Direction::left);
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Right)) {
            m_active_shape->MoveOneStep(Direction::right);
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Down)) {
            m_active_shape->MoveOneStep(Direction::down);
        } else if ((event.type == sf::Event::KeyPressed) &&
                   (event.key.code == sf::Keyboard::Up)) {
            m_active_shape->Rotate();
        }

        if (!m_active_shape->IsMovable()) {
            m_shapes_on_grid.push_back(
                std::move(m_active_shape));
            m_active_shape =
                std::move(m_shapes_in_queue.back());
            m_shapes_in_queue.pop_back();
            m_shapes_in_queue.push_front(
                std::move(ShapeFactory::create(m_grid_logic, m_grid_graphic)));
        }
    }
}

void PlayGround::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_grid_graphic, states);
    if (m_active_shape) {
        target.draw(*m_active_shape, states);
    }
    if (!m_shapes_on_grid.empty()) {
        for (const auto& shape : m_shapes_on_grid) {
            target.draw(*shape);
        }
    }
}
