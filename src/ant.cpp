#include "ant.hpp"

#include <cstdlib>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>

#include "game_of_life.hpp"

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
      SetRadius(0.01);
      break;
    case Ant::TileType::Colony:
      carrying_food_ = false;
      break;
    case Ant::TileType::Scent:
    {
      if (carrying_food_)
      {
        break;
      }

      const auto dir = GetCurrentDirection();
      Individual::Direction orth1, orth2;
      GetOrthogonalDirections(&orth1, &orth2);
      if (GetAdjacentTile(dir).type == TileType::Scent)
      {
        const auto target = GetTileCenter(GetAdjacentTile(dir));
        GoToward(target);
      }
      else if (GetAdjacentTile(orth1).type == TileType::Scent)
      {
        const auto target = GetTileCenter(GetAdjacentTile(orth1));
        GoToward(target);
      }
      else if (GetAdjacentTile(orth2).type == TileType::Scent)
      {
        const auto target = GetTileCenter(GetAdjacentTile(orth2));
        GoToward(target);
      }
      break;
    }
    default:
      break;
  }
}

void Ant::RandomDirectionAdjustment()
{
  auto angle = glm::linearRand(-max_turning_angle_, max_turning_angle_);
  SetVelocity(glm::rotate(GetVelocity(), angle));
}

void Ant::Render(sf::RenderWindow* window) const
{
  sf::CircleShape circ(GetRadius());
  carrying_food_ ? circ.setFillColor(sf::Color::Blue) : circ.setFillColor(sf::Color::Green);
  circ.setPosition(GetPosition().x, GetPosition().y);
  circ.setOrigin(circ.getRadius(), circ.getRadius());
  window->draw(circ);
}
