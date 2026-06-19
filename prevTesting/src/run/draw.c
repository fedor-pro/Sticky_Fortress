#include "draw.h"
#include <raylib.h>

void drawGuiPannel(GuiPannel guip) // draw pannel
{
    DrawRectangle(guip.startCoords.x, guip.startCoords.y, guip.canvSizeCoords.x, guip.canvSizeCoords.y, guip.backgroundColor);

    DrawRectangle(guip.startCoords.x + (guip.canvSizeCoords.x/50), (guip.startCoords.y + guip.canvSizeCoords.y) - (guip.canvSizeCoords.y/30), guip.canvSizeCoords.x - ((guip.canvSizeCoords.x/50)*2), (guip.canvSizeCoords.y/150), (Color) {255, 255, 255, 120});
    DrawRectangle(guip.startCoords.x + (guip.canvSizeCoords.x/50), (guip.startCoords.y + guip.canvSizeCoords.y) - (guip.canvSizeCoords.y/50), guip.canvSizeCoords.x - ((guip.canvSizeCoords.x/50)*2), (guip.canvSizeCoords.y/150), (Color) {255, 255, 255, 120});

    DrawRectangle(guip.startCoords.x + (guip.canvSizeCoords.x/50), guip.startCoords.y + (guip.canvSizeCoords.y/30), guip.canvSizeCoords.x - ((guip.canvSizeCoords.x/50)*2), (guip.canvSizeCoords.y/150), (Color) {255, 255, 255, 120});
    DrawRectangle(guip.startCoords.x + (guip.canvSizeCoords.x/50), guip.startCoords.y + (guip.canvSizeCoords.y/50), guip.canvSizeCoords.x - ((guip.canvSizeCoords.x/50)*2), (guip.canvSizeCoords.y/150), (Color) {255, 255, 255, 120});
}

void drawGuiText(GuiText guitx) // draw text
{
    DrawText(guitx.text, guitx.startCoords.x, guitx.startCoords.y, guitx.fontSize, guitx.fontColor);
}

void drawEntity(Entity ent, progParamsDataLord *progParamsData) // draw entity(ASCII)
{
    DrawText(ent.charValue, ent.coords.x * progParamsData->rectSize.x + 1, ent.coords.y * progParamsData->rectSize.y - 1, progParamsData->rectSize.y * 1.3, ent.drawingColor);
}

void drawItem(Item i, progParamsDataLord *progParamsData) // draw item(ASCII)
{
    DrawText(i.charValue, i.coords.x * progParamsData->rectSize.x + 1, i.coords.y * progParamsData->rectSize.y - 1, progParamsData->rectSize.y * 1.3, YELLOW);
}