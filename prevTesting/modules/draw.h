#include <stdio.h>  // for printf
#include <raylib.h> // for graphics
#include <stdlib.h> // for rand() func

#pragma once
#include "types.h"

void drawGuiPannel(GuiPannel guip) // draw pannel
{
    DrawRectangle(guip.startCoords.x, guip.startCoords.y, guip.canvSizeCoords.x, guip.canvSizeCoords.y, guip.backgroundColor);
}

void drawGuiText(GuiText guitx) // draw text
{
    DrawText(guitx.text, guitx.startCoords.x, guitx.startCoords.y, guitx.fontSize, guitx.fontColor);
}

void drawEntity(Entity ent, int rectSizeX, int rectSizeY) // draw entity(ASCII)
{
    DrawText(ent.charValue, ent.coords.x * rectSizeX + 1, ent.coords.y * rectSizeY - 1, rectSizeY * 1.3, ent.drawingColor);
}

void drawItem(Item i, int rectSizeX, int rectSizeY) // draw item(ASCII)
{
    DrawText(i.charValue, i.coords.x * rectSizeX + 1, i.coords.y * rectSizeY - 1, rectSizeY * 1.3, YELLOW);
}