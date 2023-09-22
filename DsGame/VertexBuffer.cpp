#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(size_t size)
{
    vertexes.resize(size);
    indexes.resize(size);
}

void VertexBuffer::setVertex(size_t num, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    auto& v = vertexes[num];
    v.position.x = x;
    v.position.y = y;
    v.color.r = r;
    v.color.g = g;
    v.color.b = b;
    v.color.a = a;
}

void VertexBuffer::setIndex(size_t num, int value)
{
    indexes[num] = value;
}

const SDL_Vertex* VertexBuffer::getVertexes() const
{
    return vertexes.data();
}

const int* VertexBuffer::getIndexes() const
{
    return indexes.data();
}
