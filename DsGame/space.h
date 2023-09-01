#pragma once

#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>

#define GRABBABLE_MASK_BIT (1<<31)

void space_init(cpSpace* space, int width, int height);
void space_update();
void space_destroy(cpSpace *space);

void space_mouse_move(cpSpace* space, int x, int y);
void space_mouse_down(cpSpace* space);
void space_mouse_up  (cpSpace* space);