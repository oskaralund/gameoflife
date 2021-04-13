#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <SFML/Graphics.hpp>

class Renderer;
class GameOfLife;

class Controller
{
public:
  Controller(GameOfLife*, sf::RenderWindow*, Renderer*);
  void ProcessInput();

private:
  GameOfLife* game_;
  sf::RenderWindow* window_;
  Renderer* renderer_;
  sf::Vector2f cursor_;

  void KeyPressed(sf::Event);
  void MouseButtonPressed(sf::Event);
  void MouseMoved(sf::Event);
  void MouseWheelScrolled(sf::Event);
};

#endif
