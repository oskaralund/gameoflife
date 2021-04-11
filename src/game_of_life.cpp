#include "game_of_life.hpp"

#include <glm/gtc/random.hpp>

#include "individual.hpp"
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

  if (type == 0)
  {
    tiles_[i][j].color[0] = 0;
    tiles_[i][j].color[1] = 0;
    tiles_[i][j].color[2] = 0;
    tiles_[i][j].color[3] = 255;
  }

  if (type == 1)
  {
    tiles_[i][j].color[0] = 255;
    tiles_[i][j].color[1] = 0;
    tiles_[i][j].color[2] = 255;
    tiles_[i][j].color[3] = 255;
  }
}

void GameOfLife::Move(double elapsed_time)
{
  time_accumulator_ += time_factor_*elapsed_time;

  while (time_accumulator_ - dt_ > 0)
  {
    for (auto& individual : individuals_)
    {
      individual->Move(dt_);
    }

    time_accumulator_ -= dt_;
  }

  UpdateTiles(time_factor_*elapsed_time);
}

void GameOfLife::UpdateTiles(double dt)
{
  for (int i = 0; i < num_rows_; ++i)
  {
    for (int j = 0; j < num_cols_; ++j)
    {
      if (tiles_[i][j].update)
      {
        tiles_[i][j].update(dt);
      }
    }
  }
}

void GameOfLife::AddAntColony(int num_ants)
{
  glm::dvec2 colony_pos{0.0, 0.0};
  int i,j;
  PositionToTile(colony_pos, &i, &j);
  colony_pos = GetTileCenter(i, j);
  tiles_[i][j].type = Ant::TileType::Colony;

  for (int i = 0; i < num_ants; ++i) {
    auto ant = std::make_unique<Ant>(this);

    auto theta = glm::linearRand(0.0, 2.0*3.14);
    ant->SetVelocity({0.05*glm::cos(theta), 0.05*glm::sin(theta)});
    ant->SetColonyPosition(colony_pos);
    individuals_.push_back(std::move(ant));
  }

}

void GameOfLife::PositionToTile(glm::dvec2 pos, int* i, int* j) const
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

int GameOfLife::GetNumRows() const
{
  return num_rows_;
}
int GameOfLife::GetNumCols() const
{
  return num_cols_;
}
