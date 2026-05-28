#ifndef DRAW_H
#define DRAW_H

#include <raylib.h>
#include "types.h"

void drawGuiPannel(GuiPannel guip);
void drawGuiText(GuiText guitx); // draw text
void drawEntity(Entity ent, int rectSizeX, int rectSizeY); // draw entity(ASCII)
void drawItem(Item i, int rectSizeX, int rectSizeY); // draw item(ASCII)

#endif