#include "controller.hpp"

#include <iostream>

#include "renderer.hpp"
#include "game_of_life.hpp"

Controller::Controller(GameOfLife* game, sf::RenderWindow* window, Renderer* renderer)
  : game_(game)
  , window_(window)
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

      case sf::Event::MouseButtonPressed:
        MouseButtonPressed(event);
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

    case sf::Keyboard::Up:
      game_->time_factor_ *= 2;
      break;

    case sf::Keyboard::Down:
      game_->time_factor_ /= 2;
      break;

    default:
      break;
  }
}

void Controller::MouseButtonPressed(sf::Event event)
{
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    auto cursor_pixel_pos = sf::Mouse::getPosition(*window_);
    auto cursor = window_->mapPixelToCoords(cursor_pixel_pos);
    int i, j;
    game_->PositionToTile({cursor.x, cursor.y}, &i, &j);
    int tile_type = 1;
    game_->SetTileType(i, j, tile_type);
  }
}

void Controller::MouseMoved(sf::Event event)
{
  auto new_cursor_pixel_pos = sf::Mouse::getPosition(*window_);
  auto new_cursor = window_->mapPixelToCoords(new_cursor_pixel_pos);
  auto cursor_delta = new_cursor-cursor_;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
  {
    renderer_->MoveCamera(-cursor_delta);
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    auto cursor_pixel_pos = sf::Mouse::getPosition(*window_);
    auto cursor = window_->mapPixelToCoords(cursor_pixel_pos);
    int i, j;
    game_->PositionToTile({cursor.x, cursor.y}, &i, &j);
    int tile_type = 1;
    for (int a = -10; a < 10; ++a)
    {
      for (int b = -10; b < 10; ++b)
      {
        game_->SetTileType(i+a, j+b, tile_type);
      }
    }
    game_->SetTileType(i, j, tile_type);
  }

  cursor_ = window_->mapPixelToCoords(new_cursor_pixel_pos);
}

void Controller::MouseWheelScrolled(sf::Event event)
{
  auto scroll_delta = event.mouseWheelScroll.delta;

  if (scroll_delta == -1) {
    renderer_->Zoom(0.9f);
  }

  if (scroll_delta == 1) {
    renderer_->Zoom(1.1f);
  }
}
