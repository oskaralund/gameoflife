#ifndef ANT_HPP_
#define ANT_HPP_

#include <random>

#include "agent.hpp"

struct Tile;
class GameOfLife;

namespace ants
{

enum TileType {
  Basic = 0,
  Food,
  Colony,
  Wall
};

// Holds scent and food data for a tile
struct TileData {
  float food_scent{0.0f};
  float colony_scent{0.0f};
  int food{0};
};

// Reduces the scent on a tile.
void TileUpdate(Tile*, float dt);

// Adds TileData and TileUpdate to all tiles in the grid. Must be run before
// spawning ants.
void InitializeTiles(GameOfLife* game);

class Ant : public Agent
{
public:
  using Agent::Agent;
  Ant(GameOfLife*);
  void Move(float dt) override;
  void ReactToTile(Tile*) override;

private:
  float time_accumulator_{0.0f};
  float max_turning_angle_{0.25f*3.14f};
  float no_turning_duration_{1.0f};
  float food_scent_{0.0f};
  float colony_scent_{0.0f};
  float exploration_{0.0f};
  bool carrying_food_{false};

  void RandomDirectionAdjustment();
  void LeaveScent(Tile*) const;
  void Sniff(Tile*);
  void InvestigateFood(Tile*);
  Tile* GetSmelliestAdjacentTile() const;
};

void AddAntColony(GameOfLife*, int num_ants, int row, int col);

} // namespace ants
#endif
