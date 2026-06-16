#if !defined(WORLD_H)
#define WORLD_H

#include "types.h"
#include <time.h>

void initializeWorldLandscapes(World *world);
void createWorldMap(World *world);
void deselectAllWorldMap(World *world);
void createEntities(World *world, worldParamsDataLord* worldParamsData, progParamsDataLord* progParamsData, logDataLord* logData, drawDataLord* drawData);
void createWorldFood(World *world, worldParamsDataLord* worldParamsData, drawDataLord* drawData);
void deleteWorld(World *world, worldParamsDataLord* worldParamsData);
World *initializeWorld(worldParamsDataLord* worldParamsData, progParamsDataLord* progParamsData, char* logsBarriers, Coord mapSize, logDataLord* logData, drawDataLord* drawData);

#endif