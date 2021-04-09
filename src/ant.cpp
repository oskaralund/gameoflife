#include "ant.hpp"

#include <cstdlib>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>

#include "game_of_life.hpp"

void Ant::Move(double dt)
{
  food_scent_ -= 0.1*dt;
  food_scent_ = glm::max(food_scent_, 0.0);
  colony_scent_ -= 0.1*dt;
  colony_scent_ = glm::max(colony_scent_, 0.0);
  LeaveScent();

  if (time_accumulator_ > turning_time_)
  {
    RandomDirectionAdjustment();
    time_accumulator_ = 0.0;
  }

  Individual::Move(dt);
  time_accumulator_ += dt;
}

void Ant::ReactToTile()
{
  if (!carrying_food_)
  {
    SniffForFood();
  }
  else
  {
    SniffForColony();
  }

  auto tile = GetCurrentTile();
  switch (tile->type)
  {
    case Ant::TileType::Food:
      if (carrying_food_)
        break;

      carrying_food_ = true;
      food_scent_ = 1.0;
      SetCurrentTileType(Ant::TileType::Basic);
      break;
    case Ant::TileType::Colony:
      carrying_food_ = false;
      colony_scent_ = 1.0;
      break;
    default:
      break;
  }
}

void Ant::RandomDirectionAdjustment()
{
  auto angle = glm::linearRand(-max_turning_angle_, max_turning_angle_);
  SetVelocity(glm::rotate(GetVelocity(), angle));
}

void Ant::Render(sf::RenderWindow* window) const
{
  sf::CircleShape circ(GetRadius());
  carrying_food_ ? circ.setFillColor(sf::Color::Blue) : circ.setFillColor(sf::Color::Green);
  circ.setPosition(GetPosition().x, GetPosition().y);
  circ.setOrigin(circ.getRadius(), circ.getRadius());
  window->draw(circ);
}

void Ant::SetColonyPosition(glm::dvec2 pos)
{
  colony_pos_ = pos;
}

void Ant::LeaveScent() const
{
  auto tile = GetCurrentTile();

  if (!tile->data)
  {
    auto tile_data = std::make_shared<TileData>();
    tile_data->food_scent = food_scent_;
    tile_data->colony_scent = colony_scent_;
    tile->data = tile_data;
  }
  else
  {
    auto tile_data = tile->GetData<TileData>();
    if (food_scent_ > tile_data->food_scent)
    {
      tile_data->food_scent = food_scent_;
    }

    if (colony_scent_ > tile_data->colony_scent)
    {
      tile_data->colony_scent = colony_scent_;
    }
  }
}

void Ant::SniffForFood()
{
  int my_i, my_j;
  GetCurrentTileCoords(&my_i, &my_j);
  int max_row = glm::min(my_i+1, GetGame()->GetNumRows()-1);
  int min_row = glm::max(my_i-1, 0);
  int max_col = glm::min(my_j+1, GetGame()->GetNumCols()-1);
  int min_col = glm::max(my_j-1, 0);

  const Tile* target = nullptr;
  double scent = 0.0;
  for (int i = min_row; i <= max_row; ++i)
  {
    for (int j = min_col; j <= max_col; ++j)
    {
      if (i == my_i && j == my_j)
        continue;

      const auto tile = GetGame()->GetTile(i,j);
      if (!tile->data)
        continue;

      const auto data = tile->GetData<TileData>();

      if (data->food_scent > scent)
      {
        scent = data->food_scent;
        target = tile;
      }
    }
  }

  auto my_tile = GetCurrentTile();
  if (my_tile->data)
  {
    auto my_tile_data = my_tile->GetData<TileData>();
    if (scent <= my_tile_data->food_scent && my_tile->type != Ant::TileType::Food)
    {
      my_tile_data->food_scent = 0.0;
      return;
    }
  }

  if (target)
  {
    GoToward(GetTileCenter(*target));
  }
}

void Ant::SniffForColony()
{
  int my_i, my_j;
  GetCurrentTileCoords(&my_i, &my_j);
  int max_row = glm::min(my_i+1, GetGame()->GetNumRows()-1);
  int min_row = glm::max(my_i-1, 0);
  int max_col = glm::min(my_j+1, GetGame()->GetNumCols()-1);
  int min_col = glm::max(my_j-1, 0);

  const Tile* target = nullptr;
  double scent = 0.0;
  for (int i = min_row; i <= max_row; ++i)
  {
    for (int j = min_col; j <= max_col; ++j)
    {
      const auto tile = GetGame()->GetTile(i,j);
      if (tile->type == TileType::Food)
      {
        GoToward(GetTileCenter(*tile));
        return;
      }
      if (!tile->data)
      {
        continue;
      }
      const auto data = tile->GetData<TileData>();

      if (data->colony_scent > scent)
      {
        scent = data->colony_scent;
        target = tile;
      }
    }
  }

  if (target)
  {
    GoToward(GetTileCenter(*target));
  }
}
