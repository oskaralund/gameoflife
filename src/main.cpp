#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "renderer.hpp"
#include "controller.hpp"
#include "ant.hpp"

int main()
{
  sf::RenderWindow window{ sf::VideoMode(1000, 1000), "Game of Life" };
  GameOfLife game(200,200);
  Ant::AddAntColony(&game, 1000, 100, 100);
  Renderer renderer{&window, &game};
  Controller controller(&game, &window, &renderer);

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
    controller.Render();
    window.display();
  }

  return 0;
}
