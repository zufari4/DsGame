#include "Rectangle.h"
#include <vector>
#include <string>


static const std::vector<std::string> dMap = {              
    " DDDDDDDDDDDDD         ",
    " D::::::::::::DDD      ",
    " D:::::::::::::::DD    ",
    " DDD:::::DDDDD:::::D   ",
    "   D:::::D    D:::::D  ",
    "   D:::::D     D:::::D ",
    "   D:::::D     D:::::D ",
    "   D:::::D     D:::::D ",
    "   D:::::D     D:::::D ",
    "   D:::::D     D:::::D ",
    "   D:::::D     D:::::D ",
    "   D:::::D    D:::::D  ",
    " DDD:::::DDDDD:::::D   ",
    " D:::::::::::::::DD    ",
    " D::::::::::::DDD      ",
    " DDDDDDDDDDDDD         ",
};

static const std::vector<std::string> sMap = {              
    "    SSSSSSSSSSSSSSS  ",
    "  SS:::::::::::::::S ",
    " S:::::SSSSSS::::::S ",
    " S:::::S     SSSSSSS ",
    " S:::::S             ",
    " S:::::S             ",
    "  S::::SSSS          ",
    "   SS::::::SSSSS     ",
    "     SSS::::::::SS   ",
    "        SSSSSS::::S  ",
    "             S:::::S ",
    "             S:::::S ",
    " SSSSSSS     S:::::S ",
    " S::::::SSSSSS:::::S ",
    " S:::::::::::::::SS  ",
    "  SSSSSSSSSSSSSSS    ",
};

void addD(std::vector<Entity>& objects, float x, float y);
void addS(std::vector<Entity>& objects, float x, float y);

void drawDsMap(SDL_Renderer* render, cpSpace* space, float x, float y) 
{
    static std::vector<Entity> objects;
    if (objects.empty()) {
        addD(objects, x, y);
        addS(objects, x, y);
    }
    for (auto& object : objects) {
        object.draw();
    }
}


void addD(std::vector<Entity>& objects, float x, float y)
{ 
    for (const auto& c : dMap) {
        if (c)
    }
}


void addS(std::vector<Entity>& objects, float x, float y)
{

}