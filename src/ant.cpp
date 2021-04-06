#include "ant.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>

#include "ant_colony.hpp"
#include "game_of_life.hpp"

Ant::Ant(GameOfLife* game)
  : Individual(game)
{
  SetType(Individual::Type::Ant);
}

void Ant::Move(double dt)
{
  if (time_accumulator_ > 1.0)
  {
    RandomDirectionAdjustment();
    time_accumulator_ = 0.0;
  }

  if (carrying_food_ && colony_)
  {
    auto home_velocity_ = glm::normalize(colony_->GetPosition()-GetPosition());
    SetVelocity(home_velocity_);
  }

  Individual::Move(dt);
  InteractWithObjects();
  time_accumulator_ += dt;
}

void Ant::RandomDirectionAdjustment()
{
  auto angle = glm::linearRand(-max_turning_angle_, max_turning_angle_);
  SetVelocity(glm::rotate(GetVelocity(), angle));
}

void Ant::SetColony(AntColony* colony)
{
  colony_ = colony;
}

void Ant::InteractWithObjects()
{
  for (const auto& object : GetGame()->GetObjects())
  {
    switch(object->GetType())
    {
      case Object::Type::AntFood:
      {
        const auto food_pos = object->GetPosition();
        const auto food_rad = object->GetRadius();
        const auto my_pos = GetPosition();
        const auto my_rad = GetRadius();

        if (glm::distance(food_pos, my_pos) < food_rad + my_rad)
        {
          carrying_food_ = true;
        }
        break;
      }
      case Object::Type::AntColony:
      {
        const auto colony_pos = object->GetPosition();
        const auto colony_rad = object->GetRadius();
        const auto my_pos = GetPosition();
        const auto my_rad = GetRadius();
        if (glm::distance(colony_pos, my_pos) < colony_rad + my_rad)
        {
          carrying_food_ = false;
        }
        break;
      }
      default:
      {
        break;
      }
    }
  }
}
