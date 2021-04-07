#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <glm/glm.hpp>

namespace sf
{
  class RenderWindow;
}

class Object
{
public:
  enum class Type { Basic, AntColony, AntFood, AntFoodScent };
  struct AABB {
    double xmin;
    double xmax;
    double ymin;
    double ymax;
  };

  void SetPosition(glm::dvec2);
  glm::dvec2 GetPosition() const;
  double GetRadius() const;
  virtual void Render(sf::RenderWindow*) const;
  Type GetType() const;

protected:
  void SetType(Type);

private:
  glm::dvec2 position_{0.0, 0.0};
  double radius_{0.01};
  Type type_{Type::Basic};
};

#endif
