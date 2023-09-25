#pragma once

class Physics;
struct cpBody;
struct cpShape;

class Wall
{
public:
    Wall(Physics& physics, float x1, float y1, float x2, float y2, float radius);
    ~Wall();
private:
    Physics& physics_;
    cpBody* body_;
    cpShape* shape_;
};

