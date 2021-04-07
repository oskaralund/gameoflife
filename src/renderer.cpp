#include "renderer.hpp"

#include "game_of_life.hpp"

Renderer::Renderer(sf::RenderWindow* window, GameOfLife* game)
  : window_(window)
  , game_(game)
{
  view_.setCenter(sf::Vector2f{0.0f, 0.0f});
  view_.setSize(sf::Vector2f{2.0, 2.0});
  window_->setView(view_);
  clock_.restart();
}

void Renderer::Render()
{
  sf::Time elapsed_time = clock_.getElapsedTime();
  clock_.restart();
  view_.move(view_velocity_*elapsed_time.asSeconds());
  window_->setView(view_);

  DrawTiles();
  //DrawGrid();

  for (const auto& individual : game_->individuals_)
  {
    individual->Render(window_);
  }
}

void Renderer::MoveCamera(sf::Vector2f delta)
{
  view_.move(delta);
  window_->setView(view_);
}

void Renderer::Zoom(float factor)
{
  view_.zoom(1/factor);
  window_->setView(view_);
}

void Renderer::DrawGrid()
{
  for (int i = 0; i < game_->num_rows_+1; ++i)
  {
    const auto y = -1 + 2*i/static_cast<float>(game_->num_rows_);
    sf::Vertex line[] =
    {
      sf::Vertex(sf::Vector2f(-1, y)),
      sf::Vertex(sf::Vector2f(1, y))
    };

    window_->draw(line, 2, sf::Lines);
  }

  for (int i = 0; i < game_->num_cols_+1; ++i)
  {
    const auto x = -1 + 2*i/static_cast<float>(game_->num_cols_);
    sf::Vertex line[] =
    {
      sf::Vertex(sf::Vector2f(x, -1)),
      sf::Vertex(sf::Vector2f(x, 1))
    };

    window_->draw(line, 2, sf::Lines);
  }

}

void Renderer::DrawTiles()
{
  const auto rows = game_->num_rows_;
  const auto cols = game_->num_cols_;

  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      const auto tile = game_->tiles_[i][j];
      if (tile.type == 0)
      {
        continue;
      }

      sf::RectangleShape rect{sf::Vector2f(2.0/cols, 2.0/rows)};
      rect.setPosition(-1+j*2.0/cols, -1+i*2.0/rows);

      auto fade = tile.fade ? tile.timer/tile.timer_length : 1.0;
      switch (tile.type)
      {
        case 0:
        {
          rect.setFillColor(sf::Color::Black);
          break;
        }
        case 1:
        {
          auto color = sf::Color::Red;
          color.a = fade*255;
          rect.setFillColor(color);
          break;
        }
        case 2:
        {
          auto color = sf::Color::Green;
          color.a = fade*255;
          rect.setFillColor(color);
          break;
        }
        case 3:
        {
          auto color = sf::Color::Yellow;
          color.a = fade*255;
          rect.setFillColor(color);
          break;
        }
        default:
        {
          auto color = sf::Color::White;
          color.a = fade*255;
          rect.setFillColor(color);
          break;
        }
      }
      window_->draw(rect);
    }
  }
}
