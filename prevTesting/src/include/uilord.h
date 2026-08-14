#if !defined(UILORD_H)
#define UILORD_H

#include "types.h"
#include <stdlib.h>

UILord *initializeUILord (progParamsDataLord *progParamsData, int default_font_size);
void deleteUILord (UILord *UIL);
void updateUILord (UILord *UIL, Coord mousePosition, worldParamsDataLord *worldParamsData, progParamsDataLord *progParamsData);
void drawUILord (UILord *UIL); 

#endif