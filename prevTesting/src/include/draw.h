#if !defined(DRAW_H)
#define DRAW_H

#include "types.h"

void drawGuiPannel(GuiPannel guip);
void drawGuiText(GuiText guitx);
void drawDwarf(Dwarf dw, progParamsDataLord *progParamsData);
void drawItem(Item i, progParamsDataLord *progParamsData);

#endif