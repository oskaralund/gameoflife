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
    Wall
  };

  struct TileData {
    double food_scent{0.0};
    double colony_scent{0.0};
    int food{500};
  };

  Ant(GameOfLife*);
  void Move(double dt) override;
  void ReactToTile(Tile*) override;
  void Render(sf::RenderWindow* window) const override;

  static void AddAntColony(GameOfLife*, int num_ants, int row, int col);

private:
  double time_accumulator_{0.0};
  double max_turning_angle_{0.25*3.14};
  double turning_time_{1.0};
  double food_scent_{0.0};
  double colony_scent_{0.0};
  double exploration_{0.0};
  bool carrying_food_{false};

  void RandomDirectionAdjustment();
  void LeaveScent(Tile*) const;
  void Sniff(Tile*);
  void InvestigateFood(Tile*);
};

void TileUpdate(Tile*, double);

#endif
