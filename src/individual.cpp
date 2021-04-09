#include "individual.hpp"

#include <cmath>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "game_of_life.hpp"

int Individual::instances = 0;

Individual::Individual(GameOfLife* game)
  : game_(game)
{
  id_ = Individual::instances++;
}

void Individual::Move(double dt)
{
  position_ += dt*velocity_;
  EnforcePeriodicity();
}

void Individual::EnforcePeriodicity()
{
  if (position_.x < -1)
  {
    position_.x = 1.0;
  }

  if (position_.x > 1)
  {
    position_.x = -1.0;
  }

  if (position_.y < -1)
  {
    position_.y = 1.0;
  }

  if (position_.y > 1)
  {
    position_.y = -1.0;
  }

}

void Individual::ReactToTile()
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
  game_->tiles_[i][j].type = type;
}

void Individual::SetCurrentTileTimer(double t) const
{
  int i, j;
  GetCurrentTileCoords(&i, &j);
  game_->tiles_[i][j].timer_length = t;
  game_->tiles_[i][j].timer = t;
  game_->tiles_[i][j].timer_enabled = true;
}

void Individual::SetCurrentTileFade(bool fade) const
{
  int i, j;
  GetCurrentTileCoords(&i, &j);
  game_->tiles_[i][j].fade = fade;
}

Tile* Individual::GetAdjacentTile(Direction dir) const
{
  int i, j;
  GetCurrentTileCoords(&i, &j);

  switch (dir)
  {
    case Direction::Up:
      i = (game_->num_rows_+i-1) % game_->num_rows_;
      break;
    case Direction::Down:
      i = (i+1) % game_->num_rows_;
      break;
    case Direction::Left:
      j = (game_->num_cols_+j-1) % game_->num_cols_;
      break;
    case Direction::Right:
      j = (j+1) % game_->num_cols_;
      break;
    default:
      break;
  }

  return &game_->tiles_[i][j];
}

Individual::Direction Individual::GetCurrentDirection() const
{
  const double epsilon = 1e-10;
  if (glm::length(velocity_) < epsilon)
  {
    return Direction::None;
  }

  const auto dir = glm::normalize(velocity_);

  if (glm::angle(dir, {1.0, 0.0}) < 3.14/4)
  {
    return Direction::Right;
  }
  else if (glm::angle(dir, {0.0, 1.0}) < 3.14/4)
  {
    return Direction::Down;
  }
  else if (glm::angle(dir, {-1.0, 0.0}) < 3.14/4)
  {
    return Direction::Left;
  }
  else
  {
    return Direction::Up;
  }
}

void Individual::GetOrthogonalDirections(Direction* a, Direction* b) const
{
  const auto current_dir = GetCurrentDirection();

  if (current_dir == Direction::Up || current_dir == Direction::Down)
  {
    *a = Direction::Left;
    *b = Direction::Right;
  }
  else
  {
    *a = Direction::Up;
    *b = Direction::Down;
  }
}

glm::dvec2 Individual::DirectionToVector(Direction dir) const
{
  switch (dir)
  {
    case Direction::Up:
      return {0.0, -1.0};
    case Direction::Down:
      return {0.0, 1.0};
    case Direction::Left:
      return {-1.0, 0.0};
    case Direction::Right:
      return {1.0, 0.0};
    default:
      return {0.0, 0.0};
  }
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

std::vector<Tile*> Individual::GetSurroundingTiles() const
{
  int my_i, my_j;
  GetCurrentTileCoords(&my_i, &my_j);
  int max_row = glm::min(my_i+view_distance_, game_->num_rows_-1);
  int min_row = glm::max(my_i-view_distance_, 0);
  int max_col = glm::min(my_j+view_distance_, game_->num_cols_-1);
  int min_col = glm::max(my_j-view_distance_, 0);

  std::vector<Tile*> surrounding_tiles;
  const int num_tiles = 4*(view_distance_+1)*(view_distance_+1);
  surrounding_tiles.reserve(num_tiles);

  for (int i = min_row; i <= max_row; ++i)
  {
    for (int j = min_col; j <= max_col; ++j)
    {
      if (i == my_i && j == my_j)
      {
        continue;
      }
      surrounding_tiles.push_back(&game_->tiles_[i][j]);
    }
  }

  return surrounding_tiles;
}
