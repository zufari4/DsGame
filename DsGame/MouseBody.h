#pragma once

struct cpBody;
struct cpConstraint;
typedef union SDL_Event;
class Physics;
class Camera;


class MouseBody
{
public:
    MouseBody(Physics& physics);
    void handleEvents(const SDL_Event& event);
    void setCamera(Camera& camera);

private:
    Physics& physics_;
    Camera* camera_;
    cpBody* mouseBody_;
    cpConstraint* mouseJoint_;
    float mousePntX_;
    float mousePntY_;
};