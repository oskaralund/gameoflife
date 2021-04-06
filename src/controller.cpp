#include "controller.hpp"

#include <iostream>

Controller::Controller(sf::RenderWindow* window, Renderer* renderer)
  : window_(window)
  , renderer_(renderer)
{
  auto cursor_pixel_pos = sf::Mouse::getPosition(*window_);
  cursor_ = window_->mapPixelToCoords(cursor_pixel_pos);
}

void Controller::ProcessInput() {
  sf::Event event;

  while (window_->pollEvent(event))
  {
    switch (event.type)
    {
      case sf::Event::Closed:
        window_->close();
        break;

      case sf::Event::KeyPressed:
        KeyPressed(event);
        break;

      case sf::Event::MouseMoved:
        MouseMoved(event);
        break;

      case sf::Event::MouseWheelScrolled:
        MouseWheelScrolled(event);
        break;

      default:
        break;
    }
  }
}

void Controller::KeyPressed(sf::Event event)
{
  switch (event.key.code)
  {
    case sf::Keyboard::Escape:
      window_->close();
      break;

    default:
      break;
  }
}

void Controller::MouseMoved(sf::Event event)
{
  auto new_cursor_pixel_pos = sf::Mouse::getPosition(*window_);
  auto new_cursor = window_->mapPixelToCoords(new_cursor_pixel_pos);
  auto cursor_delta = new_cursor-cursor_;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
  {
    renderer_->MoveCamera(cursor_delta);
  }

  cursor_ = window_->mapPixelToCoords(new_cursor_pixel_pos);
}

void Controller::MouseWheelScrolled(sf::Event event)
{
  auto delta = event.mouseWheelScroll.delta;

  if (delta == -1) {
    renderer_->Zoom(0.9f);
  }

  if (delta == 1) {
    renderer_->Zoom(1.1f);
  }
}
