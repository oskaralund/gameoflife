#include "ant.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>

#include "ant_colony.hpp"
#include "ant_food.hpp"
#include "ant_food_scent.hpp"
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

  if (distance_accumulator_ > 2*GetRadius() && scent_)
  {
    scent_->AddNode(GetPosition());
    distance_accumulator_ = 0.0;
  }

  Individual::Move(dt);
  InteractWithObjects();
  time_accumulator_ += dt;
  distance_accumulator_ += dt*glm::length(GetVelocity());
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
        const auto ant_food = static_cast<AntFood*>(object);
        const auto food_pos = ant_food->GetPosition();
        const auto food_rad = ant_food->GetRadius();
        const auto my_pos = GetPosition();
        const auto my_rad = GetRadius();

        if (glm::distance(food_pos, my_pos) < food_rad + my_rad)
        {
          carrying_food_ = true;
          if (!ant_food->HasBeenVisited())
          {
            ant_food->SetVisited(true);
            scent_ = GetGame()->AddAntFoodScent(ant_food);
          }
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
          if (scent_)
          {
            scent_->AddNode(GetPosition());
          }
          carrying_food_ = false;
          scent_ = nullptr;
        }
        break;
      }
      case Object::Type::AntFoodScent:
      {
        if (scent_ || carrying_food_)
        {
          break;
        }

        const auto scent = static_cast<AntFoodScent*>(object);
        const auto my_pos = GetPosition();
        const auto my_rad = GetRadius();
        const auto scent_rad = scent->GetRadius();
        const auto aabb = scent->GetAABB();

        if (my_pos.x < aabb.xmin || my_pos.x > aabb.xmax ||
            my_pos.y < aabb.ymin || my_pos.y > aabb.ymax)
        {
          break;
        }

        for (const auto& node : scent->GetNodes())
        {
          if (glm::distance(my_pos, node) < my_rad + scent_rad)
          {
            const auto food_pos = scent->GetAntFood()->GetPosition();
            auto new_direction = node == scent->GetNodes().back() ?
              food_pos - my_pos : my_pos - *(&node+1);
            SetVelocity(glm::normalize(new_direction));
          }
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

void Ant::Render(sf::RenderWindow* window) const
{
  sf::CircleShape circ(GetRadius());
  carrying_food_ ? circ.setFillColor(sf::Color::Blue) : circ.setFillColor(sf::Color::Green);
  circ.setPosition(GetPosition().x, GetPosition().y);
  circ.setOrigin(circ.getRadius(), circ.getRadius());
  window->draw(circ);
}
