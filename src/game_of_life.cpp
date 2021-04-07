#include "game_of_life.hpp"

#include <glm/gtc/random.hpp>

#include "individual.hpp"
#include "ant.hpp"

GameOfLife::GameOfLife()
  : tiles_(num_rows_)
{
  for (int i = 0; i < num_rows_; ++i) {
    tiles_[i].reserve(num_cols_);
    for (int j = 0; j < num_cols_; ++j)
    {
      const int type = 0;
      const double timer_length = 0.0;
      const double timer = 0.0;
      const bool timer_enabled = false;
      const bool fade = false;
      Tile tile = {i, j, type, timer_length, timer, timer_enabled, fade};
      tiles_[i].push_back(tile);
    }
  }

  dx_ = 2.0/num_cols_;
  dy_ = 2.0/num_rows_;
}

const Tile& GameOfLife::GetTile(int i, int j) const
{
  i = glm::clamp(i, 0, num_rows_-1);
  j = glm::clamp(j, 0, num_cols_-1);
  return tiles_[i][j];
}

void GameOfLife::SetTileType(int i, int j, int type)
{
  tiles_[i][j].type = type;
}

void GameOfLife::Move(double elapsed_time)
{
  time_accumulator_ += 10*elapsed_time;

  while (time_accumulator_ - dt_ > 0)
  {
    for (auto& individual : individuals_)
    {
      individual->Move(dt_);
      individual->ReactToTile();
    }

    time_accumulator_ -= dt_;
  }

  UpdateTiles(elapsed_time);
}

void GameOfLife::UpdateTiles(double dt)
{
  for (int i = 0; i < num_rows_; ++i)
  {
    for (int j = 0; j < num_cols_; ++j)
    {
      if (tiles_[i][j].timer_enabled)
      {
        tiles_[i][j].timer -= dt;

        if (tiles_[i][j].timer < 0)
        {
          tiles_[i][j].type = 0;
          tiles_[i][j].timer_enabled = false;
        }
      }
    }
  }
}

void GameOfLife::AddBasicIndividual()
{
  individuals_.emplace_back(std::make_unique<Individual>(this));
  individuals_.back()->SetVelocity({0.1,0.0});
}

void GameOfLife::AddAnt()
{
  individuals_.emplace_back(std::make_unique<Ant>(this));
  individuals_.back()->SetVelocity({0.05,0.0});
}

void GameOfLife::AddAntColony()
{
  const int num_ants = 10;

  for (int i = 0; i < num_ants; ++i) {
    auto ant = std::make_unique<Ant>(this);

    auto theta = glm::linearRand(0.0, 2.0*3.14);
    ant->SetVelocity({0.05*glm::cos(theta), 0.05*glm::sin(theta)});
    individuals_.push_back(std::move(ant));
  }
}

double GameOfLife::dx() const { return dx_; }
double GameOfLife::dy() const { return dy_; }
