#if !defined(UILORD_H)
#define UILORD_H

#include "types.h"
#include <stdlib.h>

UILord *initializeUILord (int windowSizeX, int windowSizeY, int text_buffer_size, int default_font_size);
void deleteUILord (UILord *UIL);
void updateUILord (UILord *UIL, Coord mousePosition, int* selectedCells, int entitiesAlive, int timer, bool isPaused);
void drawUILord (UILord *UIL); 

#endif