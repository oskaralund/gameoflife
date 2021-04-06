#ifndef ANT_HPP_
#define ANT_HPP_

#include <random>

#include "individual.hpp"

class AntColony;

class Ant : public Individual
{
public:
  Ant(GameOfLife*);
  void Move(double dt) override;
  void SetColony(AntColony*);

private:
  double time_accumulator_{0.0};
  double max_turning_angle_{0.25*3.14};
  AntColony* colony_{nullptr};
  bool carrying_food_{false};

  void RandomDirectionAdjustment();
  void InteractWithObjects();
};

#endif
