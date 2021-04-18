#include "renderer.hpp"

#include "game_of_life.hpp"

Renderer::Renderer(sf::RenderWindow* window, GameOfLife* game)
  : window_(window)
  , game_(game)
  , grid_va_(sf::Quads, game->num_cols_*game_->num_rows_*4)
{
  view_.setCenter(sf::Vector2f{0.0f, 0.0f});
  view_.setSize(sf::Vector2f{2.0, 2.0});
  window_->setView(view_);
  clock_.restart();

  grid_va_.resize(game_->num_cols_*game_->num_rows_*4);

  const auto dx = game_->dx_;
  const auto dy = game_->dy_;
  const auto padding = 0.3;
  for (int i = 0; i < game_->num_rows_; ++i)
  {
    for (int j = 0; j < game_->num_cols_; ++j)
    {
      sf::Vertex* quad = &grid_va_[(i + j*game_->num_rows_)*4];
      quad[0].position = sf::Vector2f(-1 + (j+padding)*dx, -1 + (i+padding)*dy);
      quad[1].position = sf::Vector2f(-1 + (j+1-padding)*dx, -1 + (i+padding)*dy);
      quad[2].position = sf::Vector2f(-1 + (j+1-padding)*dx, -1 + (i+1-padding)*dy);
      quad[3].position = sf::Vector2f(-1 + (j+padding)*dx, -1 + (i+1-padding)*dy);

      quad[0].color = sf::Color::Black;
      quad[1].color = sf::Color::Black;
      quad[2].color = sf::Color::Black;
      quad[3].color = sf::Color::Black;
    }
  }
}

void Renderer::Render()
{
  sf::Time elapsed_time = clock_.getElapsedTime();
  clock_.restart();
  view_.move(view_velocity_*elapsed_time.asSeconds());
  window_->setView(view_);

  DrawTiles();
  //DrawGrid();
  DrawOuterWalls();
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
  view_.zoom(factor);
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

void Renderer::DrawOuterWalls()
{
  sf::Vertex lines[] =
  {
    sf::Vertex(sf::Vector2f(-1, -1)),
    sf::Vertex(sf::Vector2f(1, -1)),
    sf::Vertex(sf::Vector2f(-1, 1)),
    sf::Vertex(sf::Vector2f(1, 1)),
    sf::Vertex(sf::Vector2f(1, -1)),
    sf::Vertex(sf::Vector2f(1, 1)),
    sf::Vertex(sf::Vector2f(-1, -1)),
    sf::Vertex(sf::Vector2f(-1, 1))
  };
  window_->draw(lines, 8, sf::Lines);
}

void Renderer::DrawTiles()
{
  for (int i = 0; i < game_->num_rows_; ++i)
  {
    for (int j = 0; j < game_->num_cols_; ++j)
    {
      sf::Vertex* quad = &grid_va_[(i + j*game_->num_rows_)*4];

      auto tile = game_->tiles_[i][j];

      sf::Color color;
      color.r = tile.color[0];
      color.g = tile.color[1];
      color.b = tile.color[2];
      color.a = tile.color[3];
      quad[0].color = color;
      quad[1].color = color;
      quad[2].color = color;
      quad[3].color = color;
    }
  }
  window_->draw(grid_va_);
}

void Renderer::ZoomAt(sf::Vector2i pixel, float factor)
{
  const sf::Vector2f before_coord{window_->mapPixelToCoords(pixel)};
  view_.zoom(factor);
  window_->setView(view_);
  const sf::Vector2f after_coord{window_->mapPixelToCoords(pixel)};
  const sf::Vector2f offset_coords{before_coord - after_coord};
  view_.move(offset_coords);
  window_->setView(view_);
}
