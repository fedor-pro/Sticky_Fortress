#if !defined(GENERATE_H)
#define GENERATE_H

#include "types.h"

bool structuresOverlayCheck(World *world, LandscapeStructure structure, Coord zoneStartCoord, int ZoneMaxLongX, int zoneMaxLongY, int l, int g);
void generateStructure(World *world);
void generateWorldStructures(World *world, int numberOfStructures);

#endif