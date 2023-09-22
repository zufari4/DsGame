#pragma once
#include <inttypes.h>


struct SDL_Vertex;


class IVertexBuffer
{
public:
    IVertexBuffer() = default;
    virtual ~IVertexBuffer() = default;
    virtual void setVertex(size_t num, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
    virtual void setIndex(size_t num, int value) = 0;
    virtual const SDL_Vertex* getVertexes() const = 0;
    virtual const int* getIndexes() const = 0;
};
