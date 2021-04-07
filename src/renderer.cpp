#include "renderer.hpp"

#include "game_of_life.hpp"

Renderer::Renderer(sf::RenderWindow* window, GameOfLife* game)
  : window_(window)
  , game_(game)
{
  view_.setCenter(sf::Vector2f{0.0f, 0.5f});
  view_.setSize(sf::Vector2f{2.0, 2.0});
  view_.zoom(30);
  window_->setView(view_);
  clock_.restart();
}

void Renderer::Render()
{
  sf::Time elapsed_time = clock_.getElapsedTime();
  clock_.restart();
  view_.move(view_velocity_*elapsed_time.asSeconds());
  window_->setView(view_);

  for (const auto& individual : game_->individuals_)
  {
    individual->Render(window_);
  }

  for (const auto& object : game_->objects_)
  {
    object->Render(window_);
  }
}

void Renderer::MoveCamera(sf::Vector2f delta)
{
  view_.move(delta);
  window_->setView(view_);
}

void Renderer::Zoom(float factor)
{
  view_.zoom(1/factor);
  window_->setView(view_);
}
