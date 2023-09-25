#pragma once

class Physics;

namespace Walls
{
    void create(Physics& physics, float width, float height, float radius); // Create segments around the edge of the screen.
    void free();
}
