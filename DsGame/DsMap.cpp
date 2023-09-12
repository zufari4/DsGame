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

// https://patorjk.com/software/taag/#p=display&h=0&c=c%2B%2B&f=Doh&t=Subscribe

static const CharMap DMap = {              
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

static CharMap SMap = {
    "   SSSSSSSSSSSSSSS   ",
    " SS:::::::::::::::S  ",
    "S:::::SSSSSS::::::S  ",
    "S:::::S     SSSSSSS  ",
    "S:::::S              ",
    "S:::::S              ",
    " S::::SSSS           ",
    "  SS::::::SSSSS      ",
    "    SSS::::::::SS    ",
    "       SSSSSS::::S   ",
    "            S:::::S  ",
    "            S:::::S  ",
    "SSSSSSS     S:::::S  ",
    "S::::::SSSSSS:::::S  ",
    "S:::::::::::::::SS   ",
    " SSSSSSSSSSSSSSS     ",
};

static CharMap bMap = {              
    "bbbbbbbb             ",
    "b::::::b             ",
    "b::::::b             ",
    "b::::::b             ",
    " b:::::b             ",
    " b:::::bbbbbbbbb     ",
    " b::::::::::::::bb   ",
    " b::::::::::::::::b  ",
    " b:::::bbbbb:::::::b ",
    " b:::::b    b::::::b ",
    " b:::::b     b:::::b ",
    " b:::::b     b:::::b ",
    " b:::::b     b:::::b ",
    " b:::::bbbbbb::::::b ",
    " b::::::::::::::::b  ",
    " b:::::::::::::::b   ",
    " bbbbbbbbbbbbbbbb    ",
};

static CharMap uMap = {
    "uuuuuu    uuuuuu   ",
    "u::::u    u::::u   ",
    "u::::u    u::::u   ",
    "u::::u    u::::u   ",
    "u::::u    u::::u   ",
    "u::::u    u::::u   ",
    "u::::u    u::::u   ",
    "u:::::uuuu:::::u   ",
    "u:::::::::::::::uu ",
    " u:::::::::::::::u ",
    "  uu::::::::uu:::u ",
    "    uuuuuuuu  uuuu ",
};

static CharMap sMap = {
    "    ssssssssss    ",
    "  ss::::::::::s   ",
    "ss:::::::::::::s  ",
    "s::::::ssss:::::s ",
    " s:::::s  ssssss  ",
    "   s::::::s       ",
    "      s::::::s    ",
    "ssssss   s:::::s  ",
    "s:::::ssss::::::s ",
    "s::::::::::::::s  ",
    " s:::::::::::ss   ",
    "  sssssssssss     ",
};

static CharMap cMap = {
    "    cccccccccccccccc ",
    "  cc:::::::::::::::c ",
    " c:::::::::::::::::c ",
    "c:::::::cccccc:::::c ",
    "c::::::c     ccccccc ",
    "c:::::c              ",
    "c:::::c              ",
    "c::::::c     ccccccc ",
    "c:::::::cccccc:::::c ",
    " c:::::::::::::::::c ",
    "  cc:::::::::::::::c ",
    "    cccccccccccccccc ",
};

static CharMap rMap = {
    "rrrrr   rrrrrrrrr    ",
    "r::::rrr:::::::::r   ",
    "r:::::::::::::::::r  ",
    "rr::::::rrrrr::::::r ",
    " r:::::r     r:::::r ",
    " r:::::r     rrrrrrr ",
    " r:::::r             ",
    " r:::::r             ",
    " r:::::r             ",
    " r:::::r             ",
    " r:::::r             ",
    " rrrrrrr             ",
};

static CharMap iMap = {
    "  iiii   ",
    " i::::i  ",
    "  iiii   ",
    "         ",
    "iiiiiii  ",
    "i:::::i  ",
    " i::::i  ",
    " i::::i  ",
    " i::::i  ",
    " i::::i  ",
    " i::::i  ",
    " i::::i  ",
    "i::::::i ",
    "i::::::i ",
    "i::::::i ",
    "iiiiiiii ",
};

static CharMap eMap = {
    "    eeeeeeeeeeee     ",
    "  ee::::::::::::ee   ",
    " e::::::eeeee:::::ee ",
    "e::::::e     e:::::e ",
    "e:::::::eeeee::::::e ",
    "e:::::::::::::::::e  ",
    "e::::::eeeeeeeeeee   ",
    "e:::::::e            ",
    "e::::::::e           ",
    " e::::::::eeeeeeee   ",
    "  ee:::::::::::::e   ",
    "    eeeeeeeeeeeeee   ",
};

static ObjMap objectsDS;
static ObjMap objectsSubscribe;

namespace DsMap
{
    float add(Render& render, cpSpace* space, Camera* camera, float start_x, float start_y, float size, float padding,
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
                if (str[col] != ' ') {
                    ObjectInfo info;
                    if (str[col] == ':') {
                        info.obj = std::make_unique<Cirle>(space, render, x, y, size / 2.0f);
                    }
                    else {
                        info.obj = std::make_unique<Rectangle>(space, render, x, y, size, size);
                    }
                    info.obj->setColor(r, g, b, a);
                    info.obj->setCamera(camera);
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

    void drawDS(Render& render, cpSpace* space, Camera* camera, float x, float y, float size, float padding)
    {
        if (objectsDS.empty()) {
            float maxX = x;
            maxX = add(render, space, camera, maxX, y, size, padding, DMap, objectsDS, 0x0084f4ff);
            maxX = add(render, space, camera, maxX, y, size, padding, SMap, objectsDS, 0xffb400ff);
        }
        for (auto& object : objectsDS) {
            object.obj->draw();
        }
    }

    void drawSubscribe(Render& render, cpSpace* space, Camera* camera, float x, float y, float size, float padding)
    {
        if (objectsSubscribe.empty()) {
            float maxX = x;
            maxX = add(render, space, camera, maxX, y, size, padding, SMap, objectsSubscribe, 0x0084faff);
            maxX = add(render, space, camera, maxX, y, size, padding, uMap, objectsSubscribe, 0xa074fbff);
            maxX = add(render, space, camera, maxX, y, size, padding, bMap, objectsSubscribe, 0xb064fcff);
            maxX = add(render, space, camera, maxX, y, size, padding, sMap, objectsSubscribe, 0xc054fdff);
            maxX = add(render, space, camera, maxX, y, size, padding, cMap, objectsSubscribe, 0xd044ffff);
            maxX = add(render, space, camera, maxX, y, size, padding, rMap, objectsSubscribe, 0xf034a6ff);
            maxX = add(render, space, camera, maxX, y, size, padding, iMap, objectsSubscribe, 0x0024b7ff);
            maxX = add(render, space, camera, maxX, y, size, padding, bMap, objectsSubscribe, 0x1014c8ff);
            maxX = add(render, space, camera, maxX, y, size, padding, eMap, objectsSubscribe, 0x2004d9ff);
        }
        for (auto& object : objectsSubscribe) {
            object.obj->draw();
        }
    }

    void resetPos()
    {
        for (auto& object : objectsDS) {
            object.obj->setPos(object.initPosX, object.initPosY);
            object.obj->resetVelocity();
            object.obj->setAngle(0);
        }
        for (auto& object : objectsSubscribe) {
            object.obj->setPos(object.initPosX, object.initPosY);
            object.obj->resetVelocity();
            object.obj->setAngle(0);
        }
    }
}


