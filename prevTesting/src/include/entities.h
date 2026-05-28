#ifndef ENTITIES_H
#define ENTITIES_H

#include <raylib.h>
#include "types.h"
#include "logging.h"

void moveEntity(Direction direct, Entity *e, Coord mapSize, LandscapeCell *map);
void restoreHungerEntity(World  *world, Entity *e, int foodOnMap, struct tm *tm, FILE *sourceLogFile);
bool findNearestFood(World * world, Entity *e, int foodOnMap);
void updateEntity(World *world, Coord mapSize, Entity *e, int timer, int foodOnMap, FILE *sourceLogFile, struct tm *tm);

#endif
