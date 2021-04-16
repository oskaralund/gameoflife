#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

class Renderer;
class GameOfLife;

class Controller
{
public:
  Controller(GameOfLife*, sf::RenderWindow*, Renderer*);
  void ProcessInput();
  void Render() const;

private:
  GameOfLife* game_;
  sf::RenderWindow* window_;
  Renderer* renderer_;
  sf::Vector2f cursor_world_pos_;
  uint8_t selected_type_ = 0;
  uint8_t brush_size_ = 5;
  sf::Color colors_[10] = {
    sf::Color::Black,
    sf::Color::White,
    sf::Color::Red,
    sf::Color::Green,
    sf::Color::Blue,
    sf::Color::Yellow,
    sf::Color::Magenta,
    sf::Color::Cyan,
    sf::Color::Cyan,
    sf::Color::Cyan
  };

  void KeyPressed(sf::Event);
  void MouseButtonPressed(sf::Event);
  void MouseMoved(sf::Event);
  void MouseWheelScrolled(sf::Event);
  sf::Vector2f GetCursorWorldPosition() const;
  void Paint() const;
  void DrawBrush() const;
};

#endif
