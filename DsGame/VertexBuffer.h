#pragma once

#include "IVertexBuffer.h"
#include <vector>
#include "SDL2/SDL_render.h"


class VertexBuffer: public IVertexBuffer
{
public:
    VertexBuffer(size_t size);
    void setVertex(size_t num, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
    void setIndex(size_t num, int value) override;
    const SDL_Vertex* getVertexes() const override;
    const int* getIndexes() const override;
private:
    std::vector<SDL_Vertex> vertexes;
    std::vector<int>    indexes;
};

