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
  void Render(sf::RenderWindow* window) const;

private:
  double time_accumulator_{0.0};
  double distance_accumulator_{0.0};
  double max_turning_angle_{0.25*3.14};
  bool carrying_food_{false};

  void RandomDirectionAdjustment();
  void InteractWithObjects();
};

#endif
