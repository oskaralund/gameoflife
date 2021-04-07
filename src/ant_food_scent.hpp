#ifndef ANT_FOOD_SCENT_HPP_
#define ANT_FOOD_SCENT_HPP_

#include "object.hpp"

#include <list>

class AntFood;

class AntFoodScent : public Object
{
public:
  AntFoodScent(AntFood*);

  void AddNode(glm::dvec2);
  const std::list<glm::dvec2>& GetNodes() const;
  AntFood* GetAntFood() const;
  AABB GetAABB() const;
  void Render(sf::RenderWindow*) const override;

private:
  std::list<glm::dvec2> nodes_;
  AntFood* food_{nullptr};
  AABB aabb_;

  void ComputeAABB();
};

#endif
