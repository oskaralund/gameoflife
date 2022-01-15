#include "individual.hpp"

#include <cmath>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "game_of_life.hpp"
#include "tile.hpp"

int Individual::instances = 0;

Individual::Individual(GameOfLife* game)
  : game_(game)
{
  id_ = Individual::instances++;
  tile_ = GetCurrentTile();
  prev_tile_ = GetCurrentTile();
}

void Individual::Move(double dt)
{
  auto tile = GetCurrentTile();
  if (tile_ != tile)
  {
    prev_tile_ = tile_;
    tile_ = tile;
    ReactToTile(tile_);
  }
  prev_position_ = position_;
  position_ += dt*velocity_;
  EnforceWalls();
}

void Individual::EnforceWalls()
{
  if (position_.x < -1)
  {
    position_.x = -1.0;
    velocity_.x = glm::abs(velocity_.x);
  }

  if (position_.x > 1)
  {
    position_.x = 1.0;
    velocity_.x = -glm::abs(velocity_.x);
  }

  if (position_.y < -1)
  {
    position_.y = -1.0;
    velocity_.y = glm::abs(velocity_.y);
  }

  if (position_.y > 1)
  {
    position_.y = 1.0;
    velocity_.y = -glm::abs(velocity_.y);
  }

}

void Individual::ReactToTile(Tile*)
{

}

void Individual::Render(sf::RenderWindow* window) const
{
  sf::CircleShape circ(radius_);
  circ.setFillColor(sf::Color::Green);
  circ.setPosition(position_.x, position_.y);
  circ.setOrigin(circ.getRadius(), circ.getRadius());
  window->draw(circ);
}

void Individual::SetPosition(glm::dvec2 p)
{
  position_ = p;
  tile_ = GetCurrentTile();
}

void Individual::SetVelocity(glm::dvec2 v)
{
  velocity_ = v;
}

glm::dvec2 Individual::GetVelocity() const
{
  return velocity_;
}

glm::dvec2 Individual::GetPosition() const
{
  return position_;
}

glm::dvec2 Individual::GetPreviousPosition() const
{
  return prev_position_;
}

GameOfLife* Individual::GetGame() const
{
  return game_;
}

double Individual::GetRadius() const
{
  return radius_;
}

void Individual::SetRadius(double r)
{
  radius_ = r;
}

void Individual::GetCurrentTileCoords(int* i, int* j) const {
  *i = static_cast<int>((1+position_.y)/game_->dy_);
  *j = static_cast<int>((1+position_.x)/game_->dx_);
  *i = glm::clamp(*i, 0, game_->num_rows_-1);
  *j = glm::clamp(*j, 0, game_->num_cols_-1);
}

Tile* Individual::GetCurrentTile() const
{
  int i, j;
  GetCurrentTileCoords(&i, &j);
  return &game_->tiles_[i][j];
}

void Individual::SetCurrentTileType(int type) const
{
  int i, j;
  GetCurrentTileCoords(&i, &j);
  game_->SetTileType(i, j, type);
}

void Individual::GoToward(glm::dvec2 target)
{
  const auto dir = target - position_;

  if (glm::length(dir) < 1e-10)
  {
    return;
  }

  velocity_ = speed_*glm::normalize(dir);
}

glm::dvec2 Individual::GetTileCenter(const Tile& tile)
{
  const auto i = tile.row;
  const auto j = tile.col;

  return {(j+0.5)*game_->dx_-1, (i+0.5)*game_->dy_-1};
}

int Individual::GetId() const
{
  return id_;
}

AdjacentTiles Individual::GetAdjacentTiles() const
{
  int i, j;
  GetCurrentTileCoords(&i, &j);
  return AdjacentTiles(game_, i, j);
}

Tile* Individual::GetPreviousTile() const
{
  return prev_tile_;
}
