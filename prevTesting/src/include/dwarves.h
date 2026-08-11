#if !defined(DWARVES_H)
#define DWARVES_H

#include "types.h"
#include <time.h>

void moveDwarf(Direction direct, Dwarf *dw, Coord mapSize, LandscapeCell *map);
void restoreHungerDwarf(World  *world, Dwarf *dw, worldParamsDataLord *worldParamsData, logDataLord *logData);
bool findNearestFood(World *world, Dwarf *dw, worldParamsDataLord *worldParamsData);
void updateDwarf(World *world, Coord mapSize, Dwarf *dw, int timer, worldParamsDataLord *worldParamsData, logDataLord *logData) ;

#endif 