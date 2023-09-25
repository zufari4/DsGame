#include "Walls.h"
#include "Wall.h"
#include <vector>
#include <memory>

namespace Walls
{
    std::vector<std::unique_ptr<Wall>> walls;

    void create(Physics& physics, float width, float height, float radius)
    {
        auto w = std::make_unique<Wall>(physics, 0, -height, 0, height, radius); walls.push_back(std::move(w));
        w = std::make_unique<Wall>(physics, width, -height, width, height, radius); walls.emplace_back(std::move(w));
        w = std::make_unique<Wall>(physics, 0, height, width, height, radius); walls.emplace_back(std::move(w));
    }

    void free()
    {
        walls.clear();
    }
}