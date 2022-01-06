#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "console.hpp"

class Renderer;
class GameOfLife;

class Controller
{
public:
  Controller(GameOfLife*, sf::RenderWindow*, Renderer*);
  void ProcessInput();
  void Render();

private:
  GameOfLife* game_;
  sf::RenderWindow* window_;
  Renderer* renderer_;
  sf::Vector2f cursor_world_pos_;
  uint8_t selected_type_ = 0;
  uint8_t brush_size_ = 1;
  sf::Font font_;
  sf::Text header_;
  bool paused_{false};
  double time_factor_{1.0};
  Console console_;

  void KeyPressed(sf::Event);
  void MouseButtonPressed(sf::Event);
  void MouseMoved(sf::Event);
  void MouseWheelScrolled(sf::Event);
  sf::Vector2f GetCursorWorldPosition() const;
  void Paint() const;
  void DrawBrush() const;
  void DrawHeader();
  void InitializeConsole();

  //Console commands
  void Quit(std::string);
};

#endif
