#include "ant_food_scent.hpp"

#include <SFML/Graphics.hpp>

#include "ant_colony.hpp"

AntFoodScent::AntFoodScent(AntFood* food) : food_(food)
{
  SetType(Object::Type::AntFoodScent);
}

void AntFoodScent::AddNode(glm::dvec2 pos) {
  nodes_.push_front(pos);
  ComputeAABB();
}

const std::list<glm::dvec2>& AntFoodScent::GetNodes() const
{
  return nodes_;
}

AntFood* AntFoodScent::GetAntFood() const
{
  return food_;
}

void AntFoodScent::Render(sf::RenderWindow* window) const
{
  for (const auto& node : nodes_)
  {
    sf::CircleShape circ(GetRadius()/2);
    circ.setFillColor(sf::Color::Red);
    circ.setPosition(node.x, node.y);
    circ.setOrigin(circ.getRadius(), circ.getRadius());
    window->draw(circ);
  }
}

Object::AABB AntFoodScent::GetAABB() const
{
  return aabb_;
}

void AntFoodScent::ComputeAABB()
{
  double xmin, xmax, ymin, ymax;
  for (const auto& node : nodes_)
  {
    if (node.x < xmin)
      xmin = node.x;

    if (node.x > xmax)
      xmax = node.x;

    if (node.y > ymax)
      ymax = node.y;

    if (node.y < ymin)
     ymin = node.y;
  }

  aabb_ = {xmin, xmax, ymin, ymax};
}
