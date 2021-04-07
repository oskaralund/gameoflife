#include "individual.hpp"

#include <cmath>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "game_of_life.hpp"

Individual::Individual(GameOfLife* game)
  : game_(game)
{}

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
  *i = static_cast<int>((1+position_.y)/game_->dy());
  *j = static_cast<int>((1+position_.x)/game_->dx());
  *i = glm::clamp(*i, 0, game_->num_rows_-1);
  *j = glm::clamp(*j, 0, game_->num_cols_-1);
}

const Tile& Individual::GetCurrentTile() const
{
  int i, j;
  GetCurrentTileCoords(&i, &j);
  return game_->GetTile(i,j);
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

const Tile& Individual::GetAdjacentTile(Direction dir) const
{
  int i, j;
  GetCurrentTileCoords(&i, &j);

  switch (dir)
  {
    case Direction::Down:
      i = (game_->num_rows_+i-1) % game_->num_rows_;
      break;
    case Direction::Up:
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

  return game_->tiles_[i][j];
}

Individual::Direction Individual::GetCurrentDirection() const
{
  const double epsilon = 1e-10;
  if (glm::length(velocity_) < epsilon)
  {
    return Direction::None;
  }

  if (glm::angle(velocity_, {1.0, 0.0}) < 45)
  {
    return Direction::Right;
  }
  else if (glm::angle(velocity_, {0.0, 1.0}) < 45)
  {
    return Direction::Down;
  }
  else if (glm::angle(velocity_, {-1.0, 0.0}) < 45)
  {
    return Direction::Left;
  } else
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
