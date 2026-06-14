#if !defined(ENTITIES_H)
#define ENTITIES_H

#include "types.h"
#include <time.h>

void moveEntity(Direction direct, Entity *e, Coord mapSize, LandscapeCell *map);
void restoreHungerEntity(World  *world, Entity *e, worldParamsDataLord* worldParamsData, logDataLord* logData);
bool findNearestFood(World *world, Entity *e, worldParamsDataLord* worldParamsData);
void updateEntity(World *world, Coord mapSize, Entity *e, int timer, worldParamsDataLord* worldParamsData, logDataLord* logData) ;

#endif 