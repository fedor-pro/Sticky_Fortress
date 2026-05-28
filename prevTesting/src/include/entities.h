#if !defined(ENTITIES_H)
#define ENTITIES_H

#include "types.h"
#include <time.h>

void moveEntity(Direction direct, Entity *e, Coord mapSize, LandscapeCell *map);
void restoreHungerEntity(World  *world, Entity *e, int foodOnMap, struct tm *tm, FILE *sourceLogFile);
bool findNearestFood(World *world, Entity *e, int foodOnMap);
void updateEntity(World *world, Coord mapSize, Entity *e, int timer, int foodOnMap, FILE *sourceLogFile, struct tm *tm) ;

#endif 