#include "Rectangle.h"
#include <vector>
#include <string>
#include <memory>


using CharMap = std::vector<std::string>;
using ObjMap  = std::vector<std::unique_ptr<Entity>>;

static const CharMap dMap = {              
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

static CharMap sMap = {              
    "      SSSSSSSSSSSSSSS  ",
    "    SS:::::::::::::::S ",
    "   S:::::SSSSSS::::::S ",
    "   S:::::S     SSSSSSS ",
    "   S:::::S             ",
    "   S:::::S             ",
    "    S::::SSSS          ",
    "     SS::::::SSSSS     ",
    "       SSS::::::::SS   ",
    "          SSSSSS::::S  ",
    "               S:::::S ",
    "               S:::::S ",
    "   SSSSSSS     S:::::S ",
    "   S::::::SSSSSS:::::S ",
    "   S:::::::::::::::SS  ",
    "    SSSSSSSSSSSSSSS    ",
};

float add(SDL_Renderer* render, cpSpace* space, float x, float y, float size, float padding, const CharMap& map, ObjMap& objects);

void drawDsMap(SDL_Renderer* render, cpSpace* space, float x, float y, float size, float padding) 
{
    static ObjMap objects;
    if (objects.empty()) {
        float maxX = add(render, space, x, y, size, padding, dMap, objects);
        add(render, space, maxX, y, size, padding, sMap, objects);
    }
    for (auto& object : objects) {
        object->draw();
    }
}

float add(SDL_Renderer* render, cpSpace* space, float start_x, float start_y, float size, float padding, const CharMap& map, ObjMap& objects)
{ 
    float maxX = 0;

    for (size_t row = 0; row < map.size(); ++row) {
        const auto& str = map[row];
        float x = row == 0 ? 0 : row * size + padding + start_x;

        for (size_t col = 0; col < str.size(); ++col) {
            if (str[col] == 'S') {
                float y = col == 0 ? 0 : col * size + padding + start_y;
                auto r = std::make_unique<Rectangle>(space, render, x, y, size, size);
                objects.push_back(std::move(r));
            }
        }

        if (x + size > maxX) {
            maxX = x + size;
        }
    }

    return maxX;
}
