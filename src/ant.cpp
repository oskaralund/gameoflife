#include "ant.hpp"

#include <cstdlib>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "tile.hpp"

void Ant::Move(double dt)
{
  Individual::Move(dt);

  food_scent_ *= glm::exp(-0.1*dt);
  colony_scent_ *= glm::exp(-0.1*dt);

  if (time_accumulator_ > turning_time_)
  {
    RandomDirectionAdjustment();
    time_accumulator_ = 0.0;
  }

  time_accumulator_ += dt;
}

void Ant::ReactToTile()
{
  LeaveScent();
  Sniff();

  auto tile = GetCurrentTile();
  switch (tile->type)
  {
    case Ant::TileType::Food:
      InvestigateFood();
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
  carrying_food_ ? circ.setFillColor(sf::Color::Green) : circ.setFillColor(sf::Color::White);
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
}

void Ant::Sniff()
{
  Tile* my_tile = GetCurrentTile();
  auto my_data = my_tile->GetData<TileData>();
  double max_scent = carrying_food_ ? my_data->colony_scent : my_data->food_scent;
  Tile* smelliest_tile = my_tile;
  auto target_tile_type = carrying_food_ ? TileType::Colony : TileType::Food;

  for (auto& tile : GetAdjacentTiles())
  {
    if (tile.type == target_tile_type)
    {
      GoToward(GetTileCenter(tile));
      return;
    }

    const auto data = tile.GetData<TileData>();

    if (!data)
      continue;

    const double scent = carrying_food_ ? data->colony_scent : data->food_scent;
    if (scent > max_scent)
    {
      max_scent = scent;
      smelliest_tile = &tile;
    }
  }

  if (smelliest_tile == my_tile)
  {
    carrying_food_ ? my_data->colony_scent = 0.0 : my_data->food_scent = 0.0;
    return;
  }

  if (smelliest_tile)
  {
    GoToward(GetTileCenter(*smelliest_tile));
  }
}

void TileUpdate(Tile* tile, double dt)
{
  auto tile_data = tile->GetData<Ant::TileData>();
  tile_data->food_scent *= glm::exp(-0.03*dt);
  tile_data->colony_scent *= glm::exp(-0.03*dt);

  if (tile->type == Ant::TileType::Basic)
  {
    tile->color[0] = tile_data->colony_scent*255;
    tile->color[1] = 0;
    tile->color[2] = tile_data->food_scent*255;
    tile->color[3] = 255;
  }
}

void Ant::InvestigateFood()
{
  auto tile = GetCurrentTile();
  auto data = tile->GetData<TileData>();

  if (!data)
    return;

  if (data->food == 1)
  {
    SetCurrentTileType(TileType::Basic);
  }
  else
  {
    data->food -= 1;
  }

  carrying_food_ = true;
  food_scent_ = 1.0;
}
