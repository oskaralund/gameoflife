#include "ant.hpp"

#include <cstdlib>
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "tile.hpp"

Ant::Ant(GameOfLife* game)
  : Individual(game)
  , generator_(GetId())
{}

void Ant::Move(double dt)
{
  Individual::Move(dt);

  food_scent_ *= glm::exp(-0.1*dt);
  colony_scent_ *= glm::exp(-0.1*dt);

  if (time_accumulator_ > turning_time_)
  {
    RandomDirectionAdjustment();

    time_accumulator_ = 0.0;
    std::exponential_distribution<double> turning_time_distribution(1.0);
    turning_time_ = turning_time_distribution(generator_);
  }

  time_accumulator_ += dt;
}

void Ant::ReactToTile(Tile* tile)
{
  Sniff(tile);

  if (tile->type != TileType::Wall)
    LeaveScent(tile);

  switch (tile->type)
  {
    case TileType::Food:
      InvestigateFood();
      break;

    case TileType::Colony:
      carrying_food_ = false;
      colony_scent_ = 1.0;
      food_scent_ = 0.0;
      break;

    case TileType::Wall:
      SetPosition(GetPreviousPosition());
      GoToward(GetTileCenter(*GetPreviousTile()));
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

void Ant::LeaveScent(Tile* tile) const
{
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

void Ant::Sniff(Tile* tile)
{
  auto data = tile->GetData<TileData>();
  double max_scent = 0.0;

  if (data)
    max_scent = carrying_food_ ? data->colony_scent : data->food_scent;

  Tile* smelliest_tile = tile;
  auto target_tile_type = carrying_food_ ? TileType::Colony : TileType::Food;

  for (auto& adjacent_tile : GetAdjacentTiles())
  {
    if (adjacent_tile.type == target_tile_type)
    {
      GoToward(GetTileCenter(adjacent_tile));
      return;
    }

    const auto adjacent_data = adjacent_tile.GetData<TileData>();

    if (!adjacent_data)
      continue;

    const double scent = carrying_food_ ? adjacent_data->colony_scent : adjacent_data->food_scent;
    if (scent > max_scent)
    {
      max_scent = scent;
      smelliest_tile = &adjacent_tile;
    }
  }

  if (smelliest_tile == tile && data)
  {
    carrying_food_ ? data->colony_scent = 0.0 : data->food_scent = 0.0;
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

bool Ant::IsSniffing() const
{
  auto tile = GetCurrentTile();
  auto data = tile->GetData<TileData>();

  if (!data)
    return false;

  if (data->food_scent > 0 && !carrying_food_)
    return true;

  if (data->colony_scent > 0 && carrying_food_)
    return true;

  return false;
}
