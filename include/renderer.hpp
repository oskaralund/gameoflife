#ifndef GAME_RENDERER_HPP_
#define GAME_RENDERER_HPP_

#include <SFML/Graphics.hpp>

class GameOfLife;
class Controller;

class Renderer {
public:
  Renderer(sf::RenderWindow*, GameOfLife*);
  void Render();
  void MoveCamera(sf::Vector2f);
  void Zoom(float factor);
  void ZoomAt(sf::Vector2i pixel, float factor);
  void MatchWindowCameraRatio();

protected:
  virtual void DrawGrid();
  virtual void DrawTiles();
  virtual void DrawAgents();
  virtual void DrawOuterWalls();
  sf::RenderWindow* window_;
  GameOfLife* game_;
  sf::VertexArray grid_va_;
  sf::VertexArray individual_va_;
  uint8_t colors_[10][4] = {
    {0, 0, 0, 255},
    {0, 255, 0, 255},
    {0, 255, 255, 255},
    {80, 80, 80, 255},
    {100, 0, 0, 255},
    {255, 0, 255, 255},
    {255, 255, 0, 255},
    {160, 82, 45, 255},
    {0, 100, 0, 255},
    {0, 0, 100, 255}
  };

private:
  sf::View view_;
  sf::Vector2f view_velocity_{.0f,.0f};
  sf::Clock clock_;

  friend Controller;
};

#endif
