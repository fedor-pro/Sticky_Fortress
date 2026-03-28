#include <stdio.h>  // for printf
#include <raylib.h> // for graphics
#include <stdlib.h> // for rand() func

#pragma once
#include "types.h"

void drawGuiPannel(GuiPannel guip) // draws pannel
{
    DrawRectangle(guip.startCoords.x, guip.startCoords.y, guip.canvSizeCoords.x, guip.canvSizeCoords.y, guip.backgroundColor);
}

void drawGuiText(GuiText guitx) // draws text
{
    DrawText(guitx.text, guitx.startCoords.x, guitx.startCoords.y, guitx.fontSize, guitx.fontColor);
}

void drawEntity(Entity ent, int rectSizeX, int rectSizeY) // draw entity(ASCII)
{
    DrawText(ent.charValue, ent.coords.x * rectSizeX + 1.8, ent.coords.y * rectSizeY - 2.5, 1, ent.drawingColor);
}

void drawResource(Resource res, int rectSizeX, int rectSizeY)
{
    DrawText(res.charValue, res.coords.x * rectSizeX + 1.5, res.coords.y * rectSizeY - 7, rectSizeX * 1.8, YELLOW);
}