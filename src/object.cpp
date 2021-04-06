#include "object.hpp"

#include <SFML/Graphics.hpp>

void Object::SetPosition(glm::dvec2 position)
{
  position_ = position;
}

glm::dvec2 Object::GetPosition() const
{
  return position_;
}

void Object::SetType(Type type)
{
  type_ = type;
}

Object::Type Object::GetType() const
{
  return type_;
}

double Object::GetRadius() const
{
  return radius_;
}

void Object::Render(sf::RenderWindow* window) const
{
  sf::CircleShape circ(radius_);
  circ.setFillColor(sf::Color::Red);
  circ.setPosition(position_.x, position_.y);
  circ.setOrigin(circ.getRadius(), circ.getRadius());
  window->draw(circ);
}
