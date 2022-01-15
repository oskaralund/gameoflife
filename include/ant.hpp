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
    float food_scent{0.0f};
    float colony_scent{0.0f};
    int food{500};
  };

  Ant(GameOfLife*);
  void Move(float dt) override;
  void ReactToTile(Tile*) override;

  static void AddAntColony(GameOfLife*, int num_ants, int row, int col);

private:
  float time_accumulator_{0.0f};
  float max_turning_angle_{0.25f*3.14f};
  float turning_time_{1.0f};
  float food_scent_{0.0f};
  float colony_scent_{0.0f};
  float exploration_{0.0f};
  bool carrying_food_{false};

  void RandomDirectionAdjustment();
  void LeaveScent(Tile*) const;
  void Sniff(Tile*);
  void InvestigateFood(Tile*);
};

void TileUpdate(Tile*, float);

#endif
