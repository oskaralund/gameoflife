#ifndef ANT_HPP_
#define ANT_HPP_

#include <random>

#include "individual.hpp"

struct Tile;

class Ant : public Individual
{
public:
  using Individual::Individual;

  enum TileType {
    Basic = 0,
    Food,
    Colony,
    FoodScent,
    HomeScent,
  };

  struct TileData {
    double food_scent{0.0};
    double colony_scent{0.0};
    int food{100};
  };

  void Move(double dt) override;
  void ReactToTile() override;
  void SetColonyPosition(glm::dvec2);
  void Render(sf::RenderWindow* window) const override;

private:
  double time_accumulator_{0.0};
  double distance_accumulator_{0.0};
  double max_turning_angle_{0.25*3.14};
  double turning_angle_{0.0};
  double turning_time_{1.0};
  double food_scent_{0.0};
  double colony_scent_{1.0};
  bool carrying_food_{false};

  void RandomDirectionAdjustment();
  void InteractWithObjects();
  void LeaveScent() const;
  void Sniff();
  void InvestigateFood();
};

void TileUpdate(Tile*, double);

#endif
