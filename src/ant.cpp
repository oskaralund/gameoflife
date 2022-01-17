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
  : Agent(game)
{
  exploration_ = glm::linearRand(0.0, 20.0);
  exploration_ = glm::exp(-exploration_);
  turning_time_ = glm::linearRand(0.0, 5.0);
}

void Ant::Move(float dt)
{
  Agent::Move(dt);

  food_scent_ *= glm::exp(-0.1*dt);
  colony_scent_ *= glm::exp(-0.1*dt);

  if (time_accumulator_ > turning_time_)
  {
    RandomDirectionAdjustment();
    turning_time_ = glm::linearRand(0.0, 5.0);
    time_accumulator_ = 0.0;
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
      InvestigateFood(tile);
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
  float max_scent = 0.0;

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

    const float scent = carrying_food_ ? adjacent_data->colony_scent : adjacent_data->food_scent;
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
    const auto rand = glm::linearRand(0.0, 1.0);
    if (rand > exploration_)
      GoToward(GetTileCenter(*smelliest_tile));
  }
}

void Ant::InvestigateFood(Tile* tile)
{
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

void Ant::AddAntColony(GameOfLife* game, int num_ants, int i, int j)
{
  int max_i = glm::min(i+1, game->num_rows()-1);
  int min_i = glm::max(i-1, 0);
  int max_j = glm::min(j+1, game->num_cols()-1);
  int min_j = glm::max(j-1, 0);

  for (int i = min_i; i <= max_i; ++i)
  {
    for (int j = min_j; j <= max_j; ++j)
    {
      game->SetTileType(i, j, Ant::TileType::Colony);
    }
  }

  const auto pos = game->GetTileCenter(i, j);

  for (int i = 0; i < num_ants; ++i) {
    auto ant = std::make_unique<Ant>(game);

    auto theta = glm::linearRand(0.0, 2.0*3.14);
    ant->SetVelocity({0.05*glm::cos(theta), 0.05*glm::sin(theta)});
    ant->SetPosition(pos);
    game->AddAgent(std::move(ant));
  }
}

void TileUpdate(Tile* tile, float dt)
{
  auto tile_data = tile->GetData<Ant::TileData>();
  tile_data->food_scent *= glm::exp(-0.03*dt);
  tile_data->colony_scent *= glm::exp(-0.03*dt);
}
