#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

class Renderer;
class GameOfLife;

class Controller {
public:
  Controller(GameOfLife*, sf::RenderWindow*, Renderer*);
  void ProcessInput();
  void Render();

protected:
  uint8_t GetSelectedBrushType() const { return selected_type_; }
  uint8_t GetBrushSize() const { return brush_size_; }
  sf::Vector2f GetCursorWorldPosition() const;
  virtual void Paint() const;
  GameOfLife* GetGame() const { return game_; }

private:
  GameOfLife* game_;
  sf::RenderWindow* window_;
  Renderer* renderer_;
  sf::Vector2f cursor_world_pos_;
  uint8_t selected_type_ = 0;
  uint8_t brush_size_ = 1;
  sf::Font font_;
  sf::Text header_;
  bool paused_{false};
  float time_factor_{1.0f};

  void KeyPressed(sf::Event);
  void MouseButtonPressed(sf::Event);
  void MouseMoved(sf::Event);
  void MouseWheelScrolled(sf::Event);
  void DrawBrush() const;
  void DrawHeader();
};

#endif
