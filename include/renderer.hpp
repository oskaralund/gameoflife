#ifndef GAME_RENDERER_HPP_
#define GAME_RENDERER_HPP_

#include <SFML/Graphics.hpp>

class GameOfLife;

class Renderer {
public:
  Renderer(sf::RenderWindow*, GameOfLife*);
  void Render();
  void MoveCamera(sf::Vector2f);
  void Zoom(float factor);
  void ZoomAt(sf::Vector2i pixel, float factor);
  void MatchWindowCameraRatio();

private:
  sf::RenderWindow* window_;
  GameOfLife* game_;
  sf::View view_;
  sf::Vector2f view_velocity_{.0f,.0f};
  sf::Clock clock_;
  sf::VertexArray grid_va_;

  void DrawGrid();
  void DrawTiles();
  void DrawOuterWalls();
};

#endif
