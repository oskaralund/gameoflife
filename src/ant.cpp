#include "ant.hpp"

#include <cstdlib>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "tile.hpp"

void Ant::Move(double dt)
{
  food_scent_ *= glm::exp(-0.1*dt);
  colony_scent_ *= glm::exp(-0.1*dt);

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
  LeaveScent();
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
      food_scent_ = 0.0;
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
  circ.setFillColor(sf::Color::Green);
  circ.setPosition(GetPosition().x, GetPosition().y);
  circ.setOrigin(circ.getRadius(), circ.getRadius());
  window->draw(circ);
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
    tile->update = TileUpdate;
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

  auto tile_data = tile->GetData<TileData>();
  const uint8_t r = tile_data->colony_scent*255;
  const uint8_t g = 0;
  const uint8_t b = tile_data->food_scent;
  const uint8_t a = 255;
  SetCurrentTileColor({r, g, b, a});
}

void Ant::SniffForFood()
{
  Tile* my_tile = GetCurrentTile();
  auto my_data = my_tile->GetData<TileData>();
  double max_scent = my_data ? my_data->food_scent : 0.0;
  Tile* smelliest_tile = my_data ? my_tile : nullptr;

  for (auto& tile : GetAdjacentTiles())
  {
    if (tile.type == TileType::Food)
    {
      GoToward(GetTileCenter(tile));
      return;
    }

    auto data = tile.GetData<TileData>();

    if (!data)
      continue;

    if (data->food_scent > max_scent)
    {
      smelliest_tile = &tile;
      max_scent = data->food_scent;
    }

  }

  if (smelliest_tile == my_tile)
  {
    auto data = my_tile->GetData<TileData>();
    data->food_scent = 0.0;
    return;
  }

  if (smelliest_tile)
  {
    GoToward(GetTileCenter(*smelliest_tile));
  }
}

void Ant::SniffForColony()
{
  const Tile* target = nullptr;
  double scent = 0.0;
  for (auto& tile : GetAdjacentTiles())
  {
    if (tile.type == TileType::Colony)
    {
      GoToward(GetTileCenter(tile));
      return;
    }

    const auto data = tile.GetData<TileData>();

    if (!data)
      continue;

    if (data->colony_scent > scent)
    {
      scent = data->colony_scent;
      target = &tile;
    }
  }

  if (target)
  {
    GoToward(GetTileCenter(*target));
  }
}

void TileUpdate(Tile* tile, double dt)
{
  auto tile_data = tile->GetData<Ant::TileData>();
  tile_data->food_scent *= glm::exp(-0.05*dt);
  tile_data->colony_scent *= glm::exp(-0.05*dt);

  if (tile->type == Ant::TileType::Basic)
  {
    tile->color[0] = tile_data->colony_scent*255;
    tile->color[1] = 0;
    tile->color[2] = tile_data->food_scent*255;
    tile->color[3] = 255;
  }
}
