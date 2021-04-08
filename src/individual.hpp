#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <glm/glm.hpp>

class GameOfLife;
struct Tile;

namespace sf
{
  class RenderWindow;
}


class Individual {
public:
  enum class Direction { Left = 0, Right, Up, Down, None };

  Individual(GameOfLife*);
  virtual void Move(double dt);
  virtual void ReactToTile();
  virtual void Render(sf::RenderWindow* window) const;
  void SetVelocity(glm::dvec2);
  void SetRadius(double);
  double GetRadius() const;
  glm::dvec2 GetPosition() const;
  glm::dvec2 GetVelocity() const;
  GameOfLife* GetGame() const;

protected:
  void SetCurrentTileType(int) const;
  void SetCurrentTileTimer(double) const;
  void SetCurrentTileColor(uint8_t color[4]) const;
  void SetCurrentTileFade(bool fade) const;
  void SetCurrentTileOwner() const;
  int GetId() const;
  const Tile& GetCurrentTile() const;
  const Tile& GetAdjacentTile(Direction) const;
  Direction GetCurrentDirection() const;
  void GetOrthogonalDirections(Direction* a, Direction* b) const;
  glm::dvec2 GetTileCenter(const Tile&);
  glm::dvec2 DirectionToVector(Direction dir) const;
  void GoToward(glm::dvec2 target);

private:
  int id_;
  glm::dvec2 position_{0,0};
  glm::dvec2 velocity_{0,0};
  double speed_{0.05};
  double radius_{0.005};
  GameOfLife* game_;
  static int instances;

  void GetCurrentTileCoords(int*, int*) const;
  void EnforcePeriodicity();
};

#endif
