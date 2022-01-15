# Game of Life
A simulation framework inspired by Conway's Game of Life. The world is grid based, but agents in the world move continuously, reacting to properties of the tiles they are on according to user defined rules.

## Ant colony example
Ants use trail pheromone to guide their friends between food and colony. Ant behavior is simple to simulate in this framework. In the animation below, light blue tiles represent colonies, green tiles represent food, and the white particles represent ants. If an ant is not carrying food, it simply wanders around randomly and drops a trail pheromone to guide food carriers to the colony. If an ant is carrying food, it sniffs its surroundings for colony trail pheromone, and also drops trail pheromone to guide ants toward the food.
![](demo.gif)
