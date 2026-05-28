#if !defined(UILORD_H)
#define UILORD_H

#include "types.h"
#include <time.h>

void initializeWorldLandscapes(World *world);
void createWorldMap(World *world);
void deselectAllWorldMap(World *world);
void createEntities(World *world, int entitiesNumber, int textBufferSize, struct tm *tm, FILE  *sourceLogFile, time_t rawTime);
void createWorldFood(World *world, int foodNumber);
void deleteWorld(World *world, int entities_number);
World *initializeWorld(int structuresNumber, int textBufferSize, char* logsBarriers, Coord mapSize, int foodOnMap, int entitiesNumber, struct tm *tm, FILE *sourceLogFile, time_t rawTime);

#endif