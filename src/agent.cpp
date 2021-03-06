#include "agent.hpp"

#include <cmath>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "game_of_life.hpp"
#include "tile.hpp"

int Agent::instances = 0;

Agent::Agent(GameOfLife* game)
    : game_(game) {
  id_ = Agent::instances++;
  tile_ = GetCurrentTile();
  prev_tile_ = GetCurrentTile();
}

void Agent::Move(float dt)
{
  auto tile = GetCurrentTile();
  if (tile_ != tile) {
    prev_tile_ = tile_;
    tile_ = tile;
    ReactToTile(tile_);
  }
  prev_position_ = position_;
  position_ += dt*velocity_;
  EnforceWalls();
}

void Agent::EnforceWalls()
{
  if (position_.x < -1) {
    position_.x = -1.0;
    velocity_.x = glm::abs(velocity_.x);
  }

  if (position_.x > 1) {
    position_.x = 1.0;
    velocity_.x = -glm::abs(velocity_.x);
  }

  if (position_.y < -1) {
    position_.y = -1.0;
    velocity_.y = glm::abs(velocity_.y);
  }

  if (position_.y > 1) {
    position_.y = 1.0;
    velocity_.y = -glm::abs(velocity_.y);
  }

}

void Agent::ReactToTile(Tile*) {

}

void Agent::set_position(glm::fvec2 p) {
  position_ = p;
  tile_ = GetCurrentTile();
}

void Agent::GetCurrentTileCoords(int* i, int* j) const {
  *i = static_cast<int>((1+position_.y)/game_->dy());
  *j = static_cast<int>((1+position_.x)/game_->dx());
  *i = glm::clamp(*i, 0, game_->num_rows()-1);
  *j = glm::clamp(*j, 0, game_->num_cols()-1);
}

Tile* Agent::GetCurrentTile() const {
  int i, j;
  GetCurrentTileCoords(&i, &j);
  return game_->GetTile(i, j);
}

void Agent::SetCurrentTileType(int type) const {
  int i, j;
  GetCurrentTileCoords(&i, &j);
  game_->SetTileType(i, j, type);
}

void Agent::GoToward(glm::fvec2 target) {
  const auto dir = target - position_;

  if (glm::length(dir) < 1e-10) {
    return;
  }

  velocity_ = speed_*glm::normalize(dir);
}

glm::fvec2 Agent::GetTileCenter(const Tile& tile) {
  const auto i = tile.row;
  const auto j = tile.col;

  return {(j+0.5)*game_->dx()-1, (i+0.5)*game_->dy()-1};
}

AdjacentTiles Agent::GetAdjacentTiles() const {
  int i, j;
  GetCurrentTileCoords(&i, &j);
  return AdjacentTiles(game_, i, j);
}

void Agent::set_velocity(glm::fvec2 v) { velocity_ = v; }
uint32_t Agent::id() const { return id_; }
Tile* Agent::GetPreviousTile() const { return prev_tile_; }
GameOfLife* Agent::game() const { return game_; }
glm::fvec2 Agent::velocity() const { return velocity_; }
glm::fvec2 Agent::position() const { return position_; }
glm::fvec2 Agent::previous_position() const { return prev_position_; }
