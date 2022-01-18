/* The Agent is one of the basic types used in the GameOfLife class.
 * It represents one agent existing inside the [-1,1]x[-1,1] square
 * on which the simulation takes place. An agent has a position
 * and a velocity. It moves and reacts to the tiles they are on and to
 * their surrounding tiles.
 *
 * This class is meant to be subclassed in order to create agents
 * with custom behavior.
*/

#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "adjacent_tiles.hpp"

class GameOfLife;
struct Tile;

class Agent {
public:
  // Constructs an agent and ties it to a GameOfLife object.
  Agent(GameOfLife*);

  // Moves the Agent based on its velocity. Also calls ReactToTile() if the
  // agent moves to a new tile. Typically if you override this function you
  // want to call Agent::Move(dt) at the top of your override.
  virtual void Move(float dt);

  // This function is called whenever the Agent moves to a new tile. By
  // default it does nothing. Override it to get custom behavior.
  virtual void ReactToTile(Tile* tile);

  void SetPosition(glm::fvec2);
  void SetVelocity(glm::fvec2);
  void SetRadius(float);
  float GetRadius() const;
  glm::fvec2 GetPosition() const;
  glm::fvec2 GetPreviousPosition() const;
  glm::fvec2 GetVelocity() const;
  GameOfLife* GetGame() const;

protected:

  // Returns a unique integer associated with the agent.
  int GetId() const;

  // Rotates the agent's velocity toward a target.
  void GoToward(glm::fvec2 target);

  // Returns a container with tiles adjacent to the agent's current tile.
  // Can be used in range based for loops, e.g.:
  //
  //   for (auto& tile : GetAdjacentTiles()) { DoSomething(tile); }
  //
  AdjacentTiles GetAdjacentTiles() const;
  void SetCurrentTileType(int) const;
  Tile* GetCurrentTile() const;
  Tile* GetPreviousTile() const;
  glm::fvec2 GetTileCenter(const Tile&);

private:
  int id_;
  int view_distance_{1};
  glm::fvec2 position_{0,0};
  glm::fvec2 velocity_{0,0};
  glm::fvec2 prev_position_{0,0};
  Tile* tile_{nullptr};
  Tile* prev_tile_{nullptr};
  float speed_{0.05f};
  GameOfLife* game_;
  static int instances;

  void EnforceWalls();
  void GetCurrentTileCoords(int*, int*) const;
};

#endif
