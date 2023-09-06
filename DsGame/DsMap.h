#pragma once

struct SDL_Renderer;
struct cpSpace;

void drawDsMap(SDL_Renderer* render, cpSpace* space, float x, float y, float size, float padding);