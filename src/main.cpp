#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "renderer.hpp"
#include "controller.hpp"

int main()
{
  sf::RenderWindow window{ sf::VideoMode(200, 200), "Game of Life" };
  GameOfLife game;
  game.AddAntColony();
  game.SetTileType(5,5,1);
  Renderer renderer{&window, &game};
  Controller controller(&window, &renderer);

  sf::Clock clock;
  sf::Time time;
  while (window.isOpen())
  {
    time = clock.getElapsedTime();
    clock.restart();
    game.Move(time.asSeconds());

    controller.ProcessInput();
    window.clear();
    renderer.Render();
    window.display();
  }

  return 0;
}
