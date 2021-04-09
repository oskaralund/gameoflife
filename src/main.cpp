#include <SFML/Graphics.hpp>

#include "game_of_life.hpp"
#include "renderer.hpp"
#include "controller.hpp"

int main()
{
  sf::RenderWindow window{ sf::VideoMode(200, 200), "Game of Life" };
  //GameOfLife game{20,20};
  //game.AddAntColony(20);
  //game.SetTileType(6,5,1);
  //GameOfLife game{100,100};
  //game.AddAntColony(200);
  //game.SetTileType(35,35,1);
  GameOfLife game(1000,1000);
  game.AddAntColony(500);
  game.SetTileType(15,15,1);
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
    window.display();
  }

  return 0;
}
