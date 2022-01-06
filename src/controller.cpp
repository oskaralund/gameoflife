#include "controller.hpp"

#include <string>
#include <iostream>

#include "renderer.hpp"
#include "game_of_life.hpp"

Controller::Controller(GameOfLife* game, sf::RenderWindow* window, Renderer* renderer)
  : game_{game}
  , window_{window}
  , renderer_{renderer}
  , console_{window, "../res/arial.ttf"}
{
  cursor_world_pos_ = GetCursorWorldPosition();

  font_.loadFromFile("../res/arial.ttf");
  header_.setFont(font_);
  header_.setCharacterSize(24);
  header_.setFillColor(sf::Color::White);
  InitializeConsole();
}

void Controller::ProcessInput() {
  console_.ProcessInput();

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

      case sf::Event::Resized:
        renderer_->MatchWindowCameraRatio();
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
    case sf::Keyboard::Space:
      if (!paused_)
      {
        paused_ = true;
        time_factor_ = game_->time_factor_;
        game_->time_factor_ = 0;
      }
      else
      {
        paused_ = false;
        game_->time_factor_ = time_factor_;
      }
      break;

    case -1:
      console_.Toggle();
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

      if (game_->time_factor_ < 20)
      {
        game_->time_factor_ += 1;
        time_factor_ = game_->time_factor_;
      }
      break;

    case sf::Keyboard::Down:
      if (paused_)
        break;

      if (game_->time_factor_ > 0)
      {
        game_->time_factor_ -= 1;
        time_factor_ = game_->time_factor_;
      }
      break;

    case sf::Keyboard::Num0:
      selected_type_ = 9;
      break;

    case sf::Keyboard::Num1:
      selected_type_ = 0;
      break;

    case sf::Keyboard::Num2:
      selected_type_ = 1;
      break;

    case sf::Keyboard::Num3:
      selected_type_ = 2;
      break;

    case sf::Keyboard::Num4:
      selected_type_ = 3;
      break;

    case sf::Keyboard::Num5:
      selected_type_ = 4;
      break;

    case sf::Keyboard::Num6:
      selected_type_ = 5;
      break;

    case sf::Keyboard::Num7:
      selected_type_ = 6;
      break;

    case sf::Keyboard::Num8:
      selected_type_ = 7;
      break;

    case sf::Keyboard::Num9:
      selected_type_ = 8;
      break;

    default:
      break;
  }
}

void Controller::MouseButtonPressed(sf::Event event)
{
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    Paint();
  }
}

void Controller::MouseMoved(sf::Event event)
{
  auto new_cursor = GetCursorWorldPosition();
  auto cursor_delta = new_cursor-cursor_world_pos_;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
  {
    renderer_->MoveCamera(-cursor_delta);
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    Paint();
  }

  cursor_world_pos_ = GetCursorWorldPosition();
}

void Controller::MouseWheelScrolled(sf::Event event)
{
  auto scroll_delta = event.mouseWheelScroll.delta;
  auto cursor_pixel_pos = sf::Mouse::getPosition(*window_);
  if (scroll_delta == -1) {
    renderer_->ZoomAt(cursor_pixel_pos, 1.1f);
  }

  if (scroll_delta == 1) {
    renderer_->ZoomAt(cursor_pixel_pos, 0.9f);
  }
}

sf::Vector2f Controller::GetCursorWorldPosition() const
{
  auto cursor_pixel_pos = sf::Mouse::getPosition(*window_);
  auto cursor_world_pos = window_->mapPixelToCoords(cursor_pixel_pos);

  return cursor_world_pos;
}

void Controller::Paint() const
{
  auto cursor = GetCursorWorldPosition();
  int i, j;
  game_->PositionToTile({cursor.x, cursor.y}, &i, &j);
  int max_i = glm::min(i+brush_size_, game_->num_rows_-1);
  int min_i = glm::max(i-brush_size_, 0);
  int max_j = glm::min(j+brush_size_, game_->num_cols_-1);
  int min_j = glm::max(j-brush_size_, 0);

  for (int i = min_i; i <= max_i; ++i)
  {
    for (int j = min_j; j <= max_j; ++j)
    {
      game_->SetTileType(i, j, selected_type_);
    }
  }
}

void Controller::Render()
{
  console_.Render();

  const auto prev_view = window_->getView();
  const auto window_size = window_->getSize();
  const auto gui_view = sf::View({0.0f, 0.0f, static_cast<float>(window_size.x), static_cast<float>(window_size.y)});
  window_->setView(gui_view);

  const auto padding = 10.0f;
  const auto rect_size = 20.0f;
  sf::RectangleShape rect{{rect_size, rect_size}};
  const auto startx = padding;
  const auto starty = window_size.y - padding - 20;

  for (int i = 0; i < 10; ++i)
  {
    rect.setPosition({startx + i*(rect_size + padding), starty});
    rect.setFillColor({game_->colors_[i][0],
                       game_->colors_[i][1],
                       game_->colors_[i][2],
                       game_->colors_[i][3]});

    if (i == selected_type_)
    {
      rect.setOutlineThickness(3);
      rect.setOutlineColor(sf::Color(250, 150, 100));
    }
    else
    {
      rect.setOutlineThickness(0);
    }

    window_->draw(rect);
  }

  DrawHeader();

  window_->setView(prev_view);

  DrawBrush();
}

void Controller::DrawBrush() const
{
  auto cursor = GetCursorWorldPosition();
  int i, j;
  game_->PositionToTile({cursor.x, cursor.y}, &i, &j);
  int max_i = glm::min(i+brush_size_, game_->num_rows_-1);
  int min_i = glm::max(i-brush_size_, 0);
  int max_j = glm::min(j+brush_size_, game_->num_cols_-1);
  int min_j = glm::max(j-brush_size_, 0);


  const auto padding = 0.3f;
  const auto dx = static_cast<float>(game_->dx_);
  const auto dy = static_cast<float>(game_->dy_);

  sf::RectangleShape rect{{dx*(1-2*padding), dy*(1-2*padding)}};
  for (int i = min_i; i <= max_i; ++i)
  {
    for (int j = min_j; j <= max_j; ++j)
    {
      rect.setPosition({-1 + (j+padding)*dy, -1 + (i+padding)*dx});
      rect.setFillColor({game_->colors_[selected_type_][0],
                         game_->colors_[selected_type_][1],
                         game_->colors_[selected_type_][2],
                         game_->colors_[selected_type_][3]});
      window_->draw(rect);
    }
  }
}

void Controller::DrawHeader()
{
  const auto time_factor = std::to_string(static_cast<int>(time_factor_));
  const auto paused = paused_ ? "Paused" : "";
  std::string string = time_factor + "x\t" + paused;
  header_.setString(string);
  window_->draw(header_);
}

void Controller::InitializeConsole()
{
  console_.AddCommand("quit", [&](std::string) -> void {window_->close();});
}
