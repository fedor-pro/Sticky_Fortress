#include <stdio.h>  // for printf
#include <stdlib.h> // for rand() func

#include "draw.h"

void drawGuiPannel(GuiPannel guip)
{
    DrawRectangle(guip.startCoords.x, guip.startCoords.y, guip.canvSizeCoords.x, guip.canvSizeCoords.y, guip.backgroundColor);
}

void drawGuiText(GuiText guitx)
{
    DrawText(guitx.text, guitx.startCoords.x, guitx.startCoords.y, guitx.fontSize, guitx.fontColor);
}

void drawEntity(Entity ent, int rectSizeX, int rectSizeY)
{
    DrawText(ent.charValue, ent.coords.x * rectSizeX + 1, ent.coords.y * rectSizeY - 1, rectSizeY * 1.3, ent.drawingColor);
}

void drawItem(Item i, int rectSizeX, int rectSizeY)
{
    DrawText(i.charValue, i.coords.x * rectSizeX + 1, i.coords.y * rectSizeY - 1, rectSizeY * 1.3, YELLOW);
}