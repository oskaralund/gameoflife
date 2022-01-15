# Game of Life
A simulation framework inspired by Conway's Game of Life. The world is grid based, but agents in the world move continuously, reacting to their environment as they transition between tiles.

## Ant colony example
Ants use trail pheromone to guide their friends between food and colony. In the animations below, light blue tiles represent ant colonies, green tiles represent food, gray tiles represent walls, and the white particles represent ants. If an ant is not carrying food, it simply wanders around randomly and drops a trail pheromone (red tiles) to guide food carriers to the colony. If an ant is carrying food, it sniffs its surroundings for colony trail pheromone, and also drops trail pheromone (blue tiles) to guide ants toward the food.

Because ants have a random exploration parameter, the food trail is gradually optimized over time as seen in the second demo below.

![](demos/demo1.gif)
![](demos/demo2.gif)

