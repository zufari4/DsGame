#include "Walls.h"
#include "Wall.h"
#include <vector>

std::vector<Wall> walls;

void createWalls(Physics& physics, float width, float height, float radius)
{ 
    walls.emplace_back(physics, 0, -height, 0, height, radius);
    walls.emplace_back(physics, width,-height, width, height, radius);
    walls.emplace_back(physics, 0, height, width, height, radius);
}