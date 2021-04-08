#include "ant.hpp"

#include <cstdlib>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>

#include "game_of_life.hpp"

void Ant::Move(double dt)
{
  if (time_accumulator_ > turning_time_)
  {
    turning_angle_ = glm::linearRand(-3.14/2, 3.14/2);
    if (carrying_food_)
    {
      GoToward(colony_pos_);
    }
    //if (!carrying_food_)
    //{
    //  RandomDirectionAdjustment();
    //}
    //else
    //{
    //  GoToward(colony_pos_);
    //  const auto distance_to_colony = glm::distance(GetPosition(), colony_pos_);
    //  auto perturbation_angle =
    //    glm::linearRand(-3.14/2*distance_to_colony, 3.14/2*distance_to_colony);
    //  SetVelocity(glm::rotate(GetVelocity(), perturbation_angle));
    //}
    time_accumulator_ = 0.0;
  }

  SetVelocity(glm::rotate(GetVelocity(), turning_angle_*dt/turning_time_));
  Individual::Move(dt);
  time_accumulator_ += dt;
}

void Ant::ReactToTile()
{
  const auto tile = GetCurrentTile();
  switch (tile.type)
  {
    case Ant::TileType::Basic:
      if (carrying_food_)
      {
        SetCurrentTileType(Ant::TileType::Scent);
        SetCurrentTileTimer(2.0);
        SetCurrentTileFade(true);
        if (tile.owner == -1)
        {
          SetCurrentTileOwner();
        }
      }
      break;
    case Ant::TileType::Food:
      carrying_food_ = true;
      break;
    case Ant::TileType::Colony:
      carrying_food_ = false;
      break;
    case Ant::TileType::Scent:
    {
      if (tile.owner == GetId())
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

void Ant::SetColonyPosition(glm::dvec2 pos)
{
  colony_pos_ = pos;
}
