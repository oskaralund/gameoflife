#include "ant.hpp"

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <limits>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "tile.hpp"

namespace ants {


Ant::Ant(GameOfLife* game)
    : Agent(game) {
  exploration_ = glm::linearRand(0.0, 20.0);
  exploration_ = glm::exp(-exploration_);
  no_turning_duration_ = glm::linearRand(0.0, 5.0);
}

void Ant::Move(float dt) {
  Agent::Move(dt);

  food_scent_ *= glm::exp(-0.1*dt);
  colony_scent_ *= glm::exp(-0.1*dt);

  if (time_accumulator_ > no_turning_duration_) {
    RandomDirectionAdjustment();
    no_turning_duration_ = glm::linearRand(0.0, 5.0);
    time_accumulator_ = 0.0;
  }

  time_accumulator_ += dt;
}

void Ant::ReactToTile(Tile* tile) {
  Sniff(tile);

  if (tile->type != TileType::Wall)
    LeaveScent(tile);

  switch (tile->type) {
    case TileType::Food:
      InvestigateFood(tile);
      break;

    case TileType::Colony:
      carrying_food_ = false;
      colony_scent_ = 1.0;
      food_scent_ = 0.0;
      break;

    case TileType::Wall:
      set_position(previous_position());
      GoToward(GetTileCenter(*GetPreviousTile()));
      break;

    default:
      break;
  }
}

void Ant::RandomDirectionAdjustment() {
  auto angle = glm::linearRand(-max_turning_angle_, max_turning_angle_);
  set_velocity(glm::rotate(velocity(), angle));
}

void Ant::LeaveScent(Tile* tile) const {
  auto tile_data = tile->GetData<TileData>();
  tile_data->food_scent = glm::max(tile_data->food_scent, food_scent_);
  tile_data->colony_scent = glm::max(tile_data->colony_scent, colony_scent_);
}

Tile* Ant::GetSmelliestAdjacentTile() const {
  Tile* smelliest_tile = GetCurrentTile();
  float max_scent = carrying_food_ ?
    smelliest_tile->GetData<TileData>()->colony_scent :
    smelliest_tile->GetData<TileData>()->food_scent;

  for (auto& tile : GetAdjacentTiles()) {
    auto data = tile.GetData<TileData>();
    auto scent = carrying_food_ ? data->colony_scent : data->food_scent;
    if (scent > max_scent) {
      max_scent = scent;
      smelliest_tile = &tile;
    }
  }

  return smelliest_tile;
}

void Ant::Sniff(Tile* tile) {
  const auto data = tile->GetData<TileData>();
  const float max_scent = carrying_food_ ? data->colony_scent : data->food_scent;
  const Tile* smelliest_tile = GetSmelliestAdjacentTile();

  if (smelliest_tile == tile) {
    if (tile->type != TileType::Food) { data->food_scent = 0.0f; }
    return;
  }

  const auto rand = glm::linearRand(0.0, 1.0);
  if (rand > exploration_) {
    const auto new_dir = glm::normalize(GetTileCenter(*smelliest_tile) - position());
    const auto cur_dir = glm::normalize(velocity());
    set_velocity(0.05f*glm::normalize(0.5f*new_dir + 0.5f*cur_dir));
    //GoToward(GetTileCenter(*smelliest_tile));
  }
}

void Ant::InvestigateFood(Tile* tile) {
  auto data = tile->GetData<TileData>();

  if (data->food == 0) {
    SetCurrentTileType(TileType::Basic);
  }
  else {
    data->food -= 1;
    carrying_food_ = true;
    food_scent_ = 1.0;
  }
}

void AddAnt(GameOfLife* game) {
  auto ant = std::make_unique<Ant>(game);
  auto theta = glm::linearRand(0.0, 2.0*3.14);
  ant->set_velocity({0.05*glm::cos(theta), 0.05*glm::sin(theta)});
  ant->set_position({0, 0});
  game->AddAgent(std::move(ant));
}

void AddAntColony(GameOfLife* game, int num_ants, int i, int j) {
  int max_i = glm::min(i+1, game->num_rows()-1);
  int min_i = glm::max(i-1, 0);
  int max_j = glm::min(j+1, game->num_cols()-1);
  int min_j = glm::max(j-1, 0);

  for (int i = min_i; i <= max_i; ++i) {
    for (int j = min_j; j <= max_j; ++j) {
      game->SetTileType(i, j, TileType::Colony);
    }
  }

  const auto pos = game->GetTileCenter(i, j);

  for (int i = 0; i < num_ants; ++i) {
    auto ant = std::make_unique<Ant>(game);

    auto theta = glm::linearRand(0.0, 2.0*3.14);
    ant->set_velocity({0.05*glm::cos(theta), 0.05*glm::sin(theta)});
    ant->set_position(pos);
    game->AddAgent(std::move(ant));
  }
}

void TileUpdate(Tile* tile, float dt) {
  auto tile_data = tile->GetData<TileData>();

  if (tile->type != TileType::Food)
    tile_data->food_scent *= glm::exp(-0.03*dt);

  if (tile->type != TileType::Colony)
    tile_data->colony_scent *= glm::exp(-0.03*dt);
}

void InitializeTiles(GameOfLife* game) {
  for (size_t i = 0; i < game->num_rows(); ++i) {
    for (size_t j = 0; j < game->num_cols(); ++j) {
       auto tile_data = std::make_shared<TileData>();
       auto tile = game->GetTile(i, j);
       tile->data = tile_data;
       tile->update = TileUpdate;
    }
  }
}


} // namespace ants
