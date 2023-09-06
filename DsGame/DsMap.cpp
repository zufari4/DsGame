#include "Rectangle.h"
#include "Circle.h"
#include <vector>
#include <string>
#include <memory>


struct ObjectInfo
{
    std::unique_ptr<Entity> obj;
    float initPosX;
    float initPosY;
};

using ObjMap = std::vector<ObjectInfo>;
using CharMap = std::vector<std::string>;

static const CharMap dMap = {              
    "SSSSSSSSSSSSS          ",
    "S::::::::::::SSS       ",
    "S:::::::::::::::SS     ",
    "SSS:::::SSSSS:::::S    ",
    "  S:::::S    S:::::S   ",
    "  S:::::S     S:::::S  ",
    "  S:::::S     S:::::S  ",
    "  S:::::S     S:::::S  ",
    "  S:::::S     S:::::S  ",
    "  S:::::S     S:::::S  ",
    "  S:::::S     S:::::S  ",
    "  S:::::S    S:::::S   ",
    "SSS:::::SSSSS:::::S    ",
    "S:::::::::::::::SS     ",
    "S::::::::::::SSS       ",
    "SSSSSSSSSSSSS          ",
};

static CharMap sMap = {              
    "   SSSSSSSSSSSSSSS ",
    " SS:::::::::::::::S",
    "S:::::SSSSSS::::::S",
    "S:::::S     SSSSSSS",
    "S:::::S            ",
    "S:::::S            ",
    " S::::SSSS         ",
    "  SS::::::SSSSS    ",
    "    SSS::::::::SS  ",
    "       SSSSSS::::S ",
    "            S:::::S",
    "            S:::::S",
    "SSSSSSS     S:::::S",
    "S::::::SSSSSS:::::S",
    "S:::::::::::::::SS ",
    " SSSSSSSSSSSSSSS   ",
};

static ObjMap objects;

namespace DsMap
{
    float add(SDL_Renderer* render, cpSpace* space, float start_x, float start_y, float size, float padding,
        const CharMap& map, ObjMap& objects, uint32_t color)
    {
        float maxX = 0;
        uint8_t r = (color >> 24) & 0xff;
        uint8_t g = (color >> 16) & 0xff;
        uint8_t b = (color >> 8) & 0xff;
        uint8_t a = color & 0xff;

        for (size_t row = 0; row < map.size(); ++row) {
            const auto& str = map[row];
            float y = row * size + start_y + padding * row;

            for (size_t col = 0; col < str.size(); ++col) {
                float x = col * size + start_x + padding * col;
                if (str[col] == 'S' || str[col] == ':') {
                    ObjectInfo info;
                    if (str[col] == 'S') {
                        info.obj = std::make_unique<Rectangle>(space, render, x, y, size, size);
                    }
                    else {
                        info.obj = std::make_unique<Cirle>(space, render, x, y, size / 2.0f);
                    }
                    info.obj->setColor(r, g, b, a);
                    info.initPosX = x;
                    info.initPosY = y;
                    objects.push_back(std::move(info));
                }
                if (x + size > maxX) {
                    maxX = x + size;
                }
            }
        }

        return maxX;
    }

    void draw(SDL_Renderer* render, cpSpace* space, float x, float y, float size, float padding)
    {
        if (objects.empty()) {
            float maxX = add(render, space, x, y, size, padding, dMap, objects, 0x0084f4ff);
            add(render, space, maxX, y, size, padding, sMap, objects, 0xffb400ff);
        }
        for (auto& object : objects) {
            object.obj->draw();
        }
    }

    void resetPos()
    {
        for (auto& object : objects) {
            object.obj->setPos(object.initPosX, object.initPosY);
            object.obj->resetVelocity();
            object.obj->setAngle(0);
        }
    }
}


