// STANDALONE MODULE
#ifndef GENERATE_H
#define GENERATE_H

#include <raylib.h>

#include "types.h"

bool structuresOverlayCheck(
    World *world, 
    LandscapeStructure structure, 
    Coord zoneStartCoord, 
    int ZoneMaxLongX, 
    int zoneMaxLongY, 
    int l, 
    int g);     //TODO возможно стоит завернуть в структуру эти параметры
void generateStructure(World *world);
void generateWorldStructures(World *world, int numberOfStructures);// Generate structures

#endif