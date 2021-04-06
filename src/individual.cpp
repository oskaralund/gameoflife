#include "individual.hpp"

#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"

Individual::Individual(GameOfLife* game)
  : game_(game)
{}

void Individual::Move(double dt)
{
  position_ += dt*velocity_;
}

void Individual::Render(sf::RenderWindow* window) const
{
  sf::CircleShape circ(radius_);
  circ.setFillColor(sf::Color::Green);
  circ.setPosition(position_.x, position_.y);
  circ.setOrigin(circ.getRadius(), circ.getRadius());
  window->draw(circ);
}

void Individual::SetVelocity(glm::dvec2 v)
{
  velocity_ = v;
}

glm::dvec2 Individual::GetVelocity() const
{
  return velocity_;
}

glm::dvec2 Individual::GetPosition() const
{
  return position_;
}

GameOfLife* Individual::GetGame() const
{
  return game_;
}

double Individual::GetRadius() const
{
  return radius_;
}

void Individual::SetType(Type type)
{
  type_ = type;
}

void Individual::SetRadius(double r)
{
  radius_ = r;
}
