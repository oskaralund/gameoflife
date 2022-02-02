#include "game_of_life.hpp"

#include <glm/gtc/random.hpp>

#include "ant.hpp"

GameOfLife::GameOfLife()
  : tiles_(num_rows_)
{
  Initialize();
}

GameOfLife::GameOfLife(int rows, int cols)
  : num_rows_(rows)
  , num_cols_(cols)
  , tiles_(rows)
{
  Initialize();
}

void GameOfLife::Initialize()
{
  for (int i = 0; i < num_rows_; ++i) {
    tiles_[i].reserve(num_cols_);
    for (int j = 0; j < num_cols_; ++j)
    {
      Tile tile = {i, j};
      tiles_[i].push_back(tile);
    }
  }

  dx_ = 2.0/num_cols_;
  dy_ = 2.0/num_rows_;
}

Tile* GameOfLife::GetTile(int i, int j)
{
  return &tiles_[i][j];
}

void GameOfLife::SetTileType(int i, int j, int type)
{
  tiles_[i][j].type = type;
}

void GameOfLife::Move(float elapsed_time)
{
  time_accumulator_ += time_factor_*elapsed_time;

  while (time_accumulator_ - dt_ > 0)
  {
    #pragma omp parallel for
    for (std::size_t i = 0; i < agents_.size(); ++i)
    {
      agents_[i]->Move(dt_);
    }

    time_accumulator_ -= dt_;
  }

  UpdateTiles(time_factor_*elapsed_time);
}

void GameOfLife::UpdateTiles(float dt)
{
  #pragma omp parallel for
  for (std::size_t i = 0; i < num_rows_; ++i)
  {
    for (std::size_t j = 0; j < num_cols_; ++j)
    {
      if (tiles_[i][j].update)
      {
        tiles_[i][j].update(&tiles_[i][j], dt);
      }
    }
  }
}

void GameOfLife::PositionToTile(glm::fvec2 pos, int* i, int* j) const
{
  *i = static_cast<int>((1+pos.y)/dy_);
  *j = static_cast<int>((1+pos.x)/dx_);
  *i = glm::clamp(*i, 0, num_rows_-1);
  *j = glm::clamp(*j, 0, num_cols_-1);
}

glm::dvec2 GameOfLife::GetTileCenter(int i, int j) const
{
  return {(j+0.5)*dx_-1, (i+0.5)*dy_-1};
}

void GameOfLife::AddAgent(std::unique_ptr<Agent> agent)
{
  agents_.push_back(std::move(agent));
}

int GameOfLife::num_rows() const { return num_rows_; }
int GameOfLife::num_cols() const { return num_cols_; }
Agent* GameOfLife::GetAgent(int i) { return agents_[i].get(); }
float GameOfLife::time_factor() const { return time_factor_; }
void GameOfLife::set_time_factor(float c) { time_factor_ = c; }
int GameOfLife::num_agents() const { return agents_.size(); }
float GameOfLife::dx() const { return dx_; }
float GameOfLife::dy() const { return dy_; }
