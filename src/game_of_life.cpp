#include "game_of_life.hpp"

#include <glm/gtc/random.hpp>

#include "individual.hpp"
#include "ant.hpp"
#include "ant_colony.hpp"
#include "ant_food.hpp"
#include "ant_food_scent.hpp"

GameOfLife::GameOfLife()
  : tiles_(num_rows_)
{
  for (auto& row : tiles_) {
    row.reserve(num_cols_);
    for (int i = 0; i < num_cols_; ++i)
    {
      row.push_back({0});
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
  const int num_ants = 100;

  auto ant_colony = std::make_unique<AntColony>();

  for (int i = 0; i < num_ants; ++i) {
    auto ant = std::make_unique<Ant>(this);
    ant_colony->AddAnt(ant.get());
    ant->SetColony(ant_colony.get());

    auto theta = glm::linearRand(0.0, 2.0*3.14);
    ant->SetVelocity({0.05*glm::cos(theta), 0.05*glm::sin(theta)});
    individuals_.push_back(std::move(ant));
  }
}

double GameOfLife::dx() const { return dx_; }
double GameOfLife::dy() const { return dy_; }
