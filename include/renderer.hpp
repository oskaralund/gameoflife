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
  void set_selected_color(int);
  sf::Vector2f GetCursorWorldPosition() const;

  virtual void DrawGrid();
  virtual void DrawTiles();
  virtual void DrawAgents();
  virtual void DrawOuterWalls();
  virtual void DrawGUI();
  virtual void DrawBrush();
  virtual void DrawBrushSelector();
  virtual void DrawHeader();

protected:
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
  sf::View world_view_;
  sf::View gui_view_;
  sf::Font font_;
  sf::Text header_;
  int selected_color_{0};
};

#endif
