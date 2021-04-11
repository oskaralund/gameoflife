#include "renderer.hpp"

#include "game_of_life.hpp"
#include "ant.hpp"

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

  for (int i = 0; i < game_->num_rows_; ++i)
  {
    for (int j = 0; j < game_->num_cols_; ++j)
    {
      sf::Vertex* quad = &grid_va_[(i + j*game_->num_cols_)*4];
      quad[0].position = sf::Vector2f(-1 + j*game_->dx_, -1 + i*game_->dy_);
      quad[1].position = sf::Vector2f(-1 + (j+1)*game_->dx_, -1 + i*game_->dy_);
      quad[2].position = sf::Vector2f(-1 + (j+1)*game_->dx_, -1 + (i+1)*game_->dy_);
      quad[3].position = sf::Vector2f(-1 + j*game_->dx_, -1 + (i+1)*game_->dy_);

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
  for (int i = 1; i < game_->num_rows_-1; ++i)
  {
    for (int j = 1; j < game_->num_cols_-1; ++j)
    {
      sf::Vertex* quad = &grid_va_[(i + j*game_->num_cols_)*4];

      auto tile = game_->tiles_[i][j];
      auto n = game_->tiles_[i-1][j];
      auto s = game_->tiles_[i+1][j];
      auto e = game_->tiles_[i][j+1];
      auto w = game_->tiles_[i][j-1];
      auto ne = game_->tiles_[i-1][j+1];
      auto nw = game_->tiles_[i-1][j-1];
      auto sw = game_->tiles_[i+1][j-1];
      auto se = game_->tiles_[i+1][j+1];

      sf::Color color;
      color.r = (tile.color[0]+n.color[0]+nw.color[0]+w.color[0])/4;
      color.g = (tile.color[1]+n.color[1]+nw.color[1]+w.color[1])/4;
      color.b = (tile.color[2]+n.color[2]+nw.color[2]+w.color[2])/4;
      color.a = (tile.color[3]+n.color[3]+nw.color[3]+w.color[3])/4;
      quad[0].color = color;

      color.r = (tile.color[0]+n.color[0]+ne.color[0]+e.color[0])/4;
      color.g = (tile.color[1]+n.color[1]+ne.color[1]+e.color[1])/4;
      color.b = (tile.color[2]+n.color[2]+ne.color[2]+e.color[2])/4;
      color.a = (tile.color[3]+n.color[3]+ne.color[3]+e.color[3])/4;
      quad[1].color = color;

      color.r = (tile.color[0]+s.color[0]+se.color[0]+e.color[0])/4;
      color.g = (tile.color[1]+s.color[1]+se.color[1]+e.color[1])/4;
      color.b = (tile.color[2]+s.color[2]+se.color[2]+e.color[2])/4;
      color.a = (tile.color[3]+s.color[3]+se.color[3]+e.color[3])/4;
      quad[2].color = color;

      color.r = (tile.color[0]+s.color[0]+sw.color[0]+w.color[0])/4;
      color.g = (tile.color[1]+s.color[1]+sw.color[1]+w.color[1])/4;
      color.b = (tile.color[2]+s.color[2]+sw.color[2]+w.color[2])/4;
      color.a = (tile.color[3]+s.color[3]+sw.color[3]+w.color[3])/4;
      quad[3].color = color;
    }
  }
  window_->draw(grid_va_);
}
