#ifndef ANT_HPP_
#define ANT_HPP_

#include <random>

#include "individual.hpp"

class AntColony;
class AntFoodScent;

class Ant : public Individual
{
public:
  Ant(GameOfLife*);

  enum TileType {
    Basic = 0,
    Food,
    Colony,
    Scent
  };

  void Move(double dt) override;
  void ReactToTile() override;
  void Render(sf::RenderWindow* window) const;
  void SetColony(AntColony*);

private:
  double time_accumulator_{0.0};
  double distance_accumulator_{0.0};
  double max_turning_angle_{0.25*3.14};
  AntColony* colony_{nullptr};
  bool carrying_food_{false};
  AntFoodScent* scent_{nullptr};

  void RandomDirectionAdjustment();
  void InteractWithObjects();
};

#endif
