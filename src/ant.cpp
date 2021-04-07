#include "ant.hpp"

#include <iostream>

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

  Individual::Move(dt);
  time_accumulator_ += dt;
}

void Ant::ReactToTile()
{
  switch (GetCurrentTile().type)
  {
    case Ant::TileType::Basic:
      if (carrying_food_)
      {
        SetCurrentTileType(Ant::TileType::Scent);
        SetCurrentTileTimer(2.0);
        SetCurrentTileFade(true);
      }
      break;
    case Ant::TileType::Food:
      carrying_food_ = true;
      SetRadius(0.02);
      break;
    case Ant::TileType::Colony:
      carrying_food_ = false;
    default:
      break;
  }
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

void Ant::Render(sf::RenderWindow* window) const
{
  sf::CircleShape circ(GetRadius());
  carrying_food_ ? circ.setFillColor(sf::Color::Blue) : circ.setFillColor(sf::Color::Green);
  circ.setPosition(GetPosition().x, GetPosition().y);
  circ.setOrigin(circ.getRadius(), circ.getRadius());
  window->draw(circ);
}
