/* The Individual is one of the basic types used in the GameOfLife class.
 * It represents one individual existing inside the [-1,1]x[-1,1] square
 * on which the simulation takes place. An individual has a position
 * and a velocity. It moves and reacts to the tiles they are on and to
 * their surrounding tiles.
 *
 * This class is meant to be subclassed in order to create individuals
 * with custom behavior.
*/

#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "adjacent_tiles.hpp"

class GameOfLife;
struct Tile;

namespace sf
{
  class RenderWindow;
}


class Individual {
public:
  // Constructs an individual and ties it to a GameOfLife object.
  Individual(GameOfLife*);

  // Moves the Individual based on its velocity. Also calls ReactToTile() if the
  // individual moves to a new tile. Typically if you override this function you
  // want to call Individual::Move(dt) at the top of your override.
  virtual void Move(double dt);

  // This function is called whenever the Individual moves to a new tile. By
  // default it does nothing. Override it to get custom behavior.
  virtual void ReactToTile(Tile* tile);

  // Specifies how to render the Individual. Defaults to a green circle.
  virtual void Render(sf::RenderWindow* window) const;

  void SetPosition(glm::dvec2);
  void SetVelocity(glm::dvec2);
  void SetRadius(double);
  double GetRadius() const;
  glm::dvec2 GetPosition() const;
  glm::dvec2 GetPreviousPosition() const;
  glm::dvec2 GetVelocity() const;
  GameOfLife* GetGame() const;

protected:

  // Returns a unique integer associated with the individual.
  int GetId() const;

  // Rotates the individual's velocity toward a target.
  void GoToward(glm::dvec2 target);

  // Returns a container with tiles adjacent to the individual's current tile.
  // Can be used in range based for loops, e.g.:
  //
  //   for (auto& tile : GetAdjacentTiles()) { DoSomething(tile); }
  //
  AdjacentTiles GetAdjacentTiles() const;
  void SetCurrentTileType(int) const;
  void SetCurrentTileColor(const std::array<uint8_t, 4>& color) const;
  Tile* GetCurrentTile() const;
  Tile* GetPreviousTile() const;
  glm::dvec2 GetTileCenter(const Tile&);

private:
  int id_;
  int view_distance_{1};
  glm::dvec2 position_{0,0};
  glm::dvec2 velocity_{0,0};
  glm::dvec2 prev_position_{0,0};
  Tile* tile_{nullptr};
  Tile* prev_tile_{nullptr};
  double speed_{0.05};
  double radius_{0.001};
  GameOfLife* game_;
  static int instances;

  void EnforceWalls();
  void GetCurrentTileCoords(int*, int*) const;
};

#endif
