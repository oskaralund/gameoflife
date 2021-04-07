#include "game_of_life.hpp"

#include <glm/gtc/random.hpp>

#include "individual.hpp"
#include "ant.hpp"
#include "ant_colony.hpp"
#include "ant_food.hpp"
#include "ant_food_scent.hpp"

void GameOfLife::Move(double elapsed_time)
{
  time_accumulator_ += 10*elapsed_time;

  while (time_accumulator_ - dt_ > 0)
  {
    for (auto& individual : individuals_)
    {
      individual->Move(dt_);
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
  individuals_.back()->SetVelocity({5.0,0.0});
}

void GameOfLife::AddAntColony()
{
  const int num_ants = 10000;

  auto ant_colony = std::make_unique<AntColony>();

  for (int i = 0; i < num_ants; ++i) {
    auto ant = std::make_unique<Ant>(this);
    ant_colony->AddAnt(ant.get());
    ant->SetColony(ant_colony.get());

    auto theta = glm::linearRand(0.0, 2.0*3.14);
    ant->SetVelocity({glm::cos(theta), glm::sin(theta)});
    individuals_.push_back(std::move(ant));
  }

  objects_.push_back(std::move(ant_colony));
}

void GameOfLife::AddAntFood(glm::dvec2 position)
{
  auto ant_food = std::make_unique<AntFood>();
  ant_food->SetPosition(position);
  objects_.push_back(std::move(ant_food));
}

std::vector<Object*> GameOfLife::GetObjects() const {
  std::vector<Object*> objects;
  objects.reserve(objects_.size());

  for (const auto& object : objects_)
  {
    objects.push_back(object.get());
  }

  return objects;
}

AntFoodScent* GameOfLife::AddAntFoodScent(AntFood* food)
{
  auto ant_food_scent = std::make_unique<AntFoodScent>(food);
  auto ret = ant_food_scent.get();
  objects_.push_back(std::move(ant_food_scent));
  return ret;
}
