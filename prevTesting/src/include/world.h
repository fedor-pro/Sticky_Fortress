#if !defined(WORLD_H)
#define WORLD_H

#include "types.h"
#include <time.h>

void initializeWorldLandscapes(World *world);
void createWorldMap(World *world);
void deselectAllWorldMap(World *world);
void createEntities(World *world, worldParamsDataLord* worldParamsData, progParamsDataLord* progParamsData, struct tm *tm, FILE  *sourceLogFile, time_t rawTime, drawDataLord* drawData);
void createWorldFood(World *world, worldParamsDataLord* worldParamsData);
void deleteWorld(World *world, worldParamsDataLord* worldParamsData);
World *initializeWorld(worldParamsDataLord* worldParamsData, progParamsDataLord* progParamsData, char* logsBarriers, Coord mapSize, struct tm *tm, FILE *sourceLogFile, time_t rawTime, drawDataLord* drawData);

#endif