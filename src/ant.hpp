#ifndef ANT_HPP_
#define ANT_HPP_

#include <random>

#include "individual.hpp"

class Ant : public Individual
{
public:
  using Individual::Individual;

  enum TileType {
    Basic = 0,
    Food,
    Colony,
    Scent
  };

  void Move(double dt) override;
  void ReactToTile() override;
  void SetColonyPosition(glm::dvec2);
  void Render(sf::RenderWindow* window) const;

private:
  double time_accumulator_{0.0};
  double distance_accumulator_{0.0};
  double max_turning_angle_{0.25*3.14};
  double turning_angle_{0.0};
  double turning_time_ = 5.0;
  bool carrying_food_{false};
  glm::dvec2 colony_pos_{0.0, 0.0};

  void RandomDirectionAdjustment();
  void InteractWithObjects();
};

#endif
