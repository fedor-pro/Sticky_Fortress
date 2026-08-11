#include "draw.h"
#include <raylib.h>

void drawGuiPannel(GuiPannel guip) // Draw pannel
{
    DrawRectangle(guip.startCoords.x, guip.startCoords.y, guip.canvSizeCoords.x, guip.canvSizeCoords.y, guip.backgroundColor);

    DrawRectangle(guip.startCoords.x + (guip.canvSizeCoords.x/50), (guip.startCoords.y + guip.canvSizeCoords.y) - (guip.canvSizeCoords.y/30), guip.canvSizeCoords.x - ((guip.canvSizeCoords.x/50)*2), 1, (Color) {255, 255, 255, 120});
    DrawRectangle(guip.startCoords.x + (guip.canvSizeCoords.x/50), (guip.startCoords.y + guip.canvSizeCoords.y) - (guip.canvSizeCoords.y/30) + 2, guip.canvSizeCoords.x - ((guip.canvSizeCoords.x/50)*2), 1, (Color) {255, 255, 255, 120});

    DrawRectangle(guip.startCoords.x + (guip.canvSizeCoords.x/50), guip.startCoords.y + (guip.canvSizeCoords.y/30), guip.canvSizeCoords.x - ((guip.canvSizeCoords.x/50)*2), 1, (Color) {255, 255, 255, 120});
    DrawRectangle(guip.startCoords.x + (guip.canvSizeCoords.x/50), guip.startCoords.y + (guip.canvSizeCoords.y/30) + 2, guip.canvSizeCoords.x - ((guip.canvSizeCoords.x/50)*2), 1, (Color) {255, 255, 255, 120});
}

void drawGuiText(GuiText guitx) // Draw text
{
    DrawText(guitx.text, guitx.startCoords.x, guitx.startCoords.y, guitx.fontSize, guitx.fontColor);
}

void drawDwarf(Dwarf dw, progParamsDataLord *progParamsData) // Draw dwarf(ASCII)
{
    DrawText(dw.charValue, dw.coords.x * progParamsData->rectSize.x + 1, dw.coords.y * progParamsData->rectSize.y - 1, progParamsData->rectSize.y * 1.3, dw.drawingColor);
}

void drawItem(Item i, progParamsDataLord *progParamsData) // Draw item(ASCII)
{
    DrawText(i.charValue, i.coords.x * progParamsData->rectSize.x + 1, i.coords.y * progParamsData->rectSize.y - 1, progParamsData->rectSize.y * 1.3, YELLOW);
}