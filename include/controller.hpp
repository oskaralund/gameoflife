#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

class Renderer;
class GameOfLife;

class Controller {
public:
  Controller(GameOfLife*, sf::RenderWindow*, Renderer*);
  void ProcessInput();
  virtual void Paint() const;

  GameOfLife* game() const;
  Renderer* renderer() const;
  uint8_t brush_size() const;
  uint8_t selected_brush_type() const;

private:
  GameOfLife* game_;
  sf::RenderWindow* window_;
  Renderer* renderer_;
  sf::Vector2f cursor_world_pos_;
  uint8_t selected_type_ = 0;
  uint8_t brush_size_ = 1;
  bool paused_{false};
  float time_factor_{1.0f};

  void KeyPressed(sf::Event);
  void MouseButtonPressed(sf::Event);
  void MouseMoved(sf::Event);
  void MouseWheelScrolled(sf::Event);
};

#endif
