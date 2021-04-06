#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <SFML/Graphics.hpp>

#include "renderer.hpp"

class Controller
{
public:
  Controller(sf::RenderWindow*, Renderer*);
  void ProcessInput();

private:
  sf::RenderWindow* window_;
  Renderer* renderer_;
  sf::Vector2f cursor_;

  void KeyPressed(sf::Event);
  void MouseMoved(sf::Event);
  void MouseWheelScrolled(sf::Event);
};

#endif
