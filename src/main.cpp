#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "ant_renderer.hpp"
#include "ant_controller.hpp"
#include "ant.hpp"

int main()
{
  sf::RenderWindow window{ sf::VideoMode(500, 500), "Game of Life" };
  window.setFramerateLimit(60);
  GameOfLife game(200,200);
  ants::InitializeTiles(&game);
  ants::AddAntColony(&game, 1000, 100, 100);
  ants::AntRenderer renderer{&window, &game};
  ants::AntController controller(&game, &window, &renderer);

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
