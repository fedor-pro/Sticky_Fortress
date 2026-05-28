#if !defined(DRAW_H)
#define DRAW_H

#include "types.h"

void drawGuiPannel(GuiPannel guip);
void drawGuiText(GuiText guitx);
void drawEntity(Entity ent, int rectSizeX, int rectSizeY);
void drawItem(Item i, int rectSizeX, int rectSizeY);

#endif