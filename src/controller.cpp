#include "controller.hpp"

#include <string>
#include <iostream>

#include "renderer.hpp"
#include "game_of_life.hpp"

Controller::Controller(GameOfLife* game, sf::RenderWindow* window, Renderer* renderer)
    : game_{game}
    , window_{window}
    , renderer_{renderer} {
  cursor_world_pos_ = renderer_->GetCursorWorldPosition();
}

void Controller::ProcessInput() {
  sf::Event event;

  while (window_->pollEvent(event)) {
    switch (event.type) {
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

      case sf::Event::Resized:
        renderer_->HandleWindowResize();
        break;

      default:
        break;
    }
  }
}

void Controller::KeyPressed(sf::Event event) {
  switch (event.key.code) {
    case sf::Keyboard::Space:
      if (!paused_) {
        paused_ = true;
        time_factor_ = game_->time_factor();
        game_->set_time_factor(0);
      }
      else {
        paused_ = false;
        game_->set_time_factor(time_factor_);
      }
      break;

    case sf::Keyboard::Escape:
      window_->close();
      break;

    case sf::Keyboard::Q:
      window_->close();
      break;

    case sf::Keyboard::Up:
      if (paused_)
        break;

      if (game_->time_factor() < 20) {
        game_->set_time_factor(game_->time_factor()+1);
        time_factor_ = game_->time_factor();
      }
      break;

    case sf::Keyboard::Down:
      if (paused_)
        break;

      if (game_->time_factor() > 0) {
        game_->set_time_factor(game_->time_factor()-1);
        time_factor_ = game_->time_factor();
      }
      break;

    case sf::Keyboard::Num0:
      renderer_->set_selected_color(9);
      selected_type_ = 9;
      break;

    case sf::Keyboard::Num1:
      renderer_->set_selected_color(0);
      selected_type_ = 0;
      break;

    case sf::Keyboard::Num2:
      renderer_->set_selected_color(1);
      selected_type_ = 1;
      break;

    case sf::Keyboard::Num3:
      renderer_->set_selected_color(2);
      selected_type_ = 2;
      break;

    case sf::Keyboard::Num4:
      renderer_->set_selected_color(3);
      selected_type_ = 3;
      break;

    case sf::Keyboard::Num5:
      renderer_->set_selected_color(4);
      selected_type_ = 4;
      break;

    case sf::Keyboard::Num6:
      renderer_->set_selected_color(5);
      selected_type_ = 5;
      break;

    case sf::Keyboard::Num7:
      renderer_->set_selected_color(6);
      selected_type_ = 6;
      break;

    case sf::Keyboard::Num8:
      renderer_->set_selected_color(7);
      selected_type_ = 7;
      break;

    case sf::Keyboard::Num9:
      renderer_->set_selected_color(8);
      selected_type_ = 8;
      break;

    default:
      break;
  }
}

void Controller::MouseButtonPressed(sf::Event event) {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    Paint();
  }
}

void Controller::MouseMoved(sf::Event event) {
  auto new_cursor = renderer_->GetCursorWorldPosition();
  auto cursor_delta = new_cursor-cursor_world_pos_;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
    renderer_->MoveCamera(-cursor_delta);
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    Paint();
  }

  cursor_world_pos_ = renderer_->GetCursorWorldPosition();
}

void Controller::MouseWheelScrolled(sf::Event event) {
  auto scroll_delta = event.mouseWheelScroll.delta;
  auto cursor_pixel_pos = sf::Mouse::getPosition(*window_);
  if (scroll_delta == -1) {
    renderer_->ZoomAt(cursor_pixel_pos, 1.1f);
  }

  if (scroll_delta == 1) {
    renderer_->ZoomAt(cursor_pixel_pos, 0.9f);
  }
}

void Controller::Paint() const {
  auto cursor = renderer_->GetCursorWorldPosition();
  auto [i, j] = game_->PositionToTile({cursor.x, cursor.y});
  int max_i = glm::min(i+brush_size_, game_->num_rows()-1);
  int min_i = glm::max(i-brush_size_, 0);
  int max_j = glm::min(j+brush_size_, game_->num_cols()-1);
  int min_j = glm::max(j-brush_size_, 0);

  for (int i = min_i; i <= max_i; ++i) {
    for (int j = min_j; j <= max_j; ++j) {
      game_->SetTileType(i, j, selected_type_);
    }
  }
}

Renderer* Controller::renderer() const { return renderer_; }
GameOfLife* Controller::game() const { return game_; }
uint8_t Controller::brush_size() const { return brush_size_; }
uint8_t Controller::selected_brush_type() const { return selected_type_; }
