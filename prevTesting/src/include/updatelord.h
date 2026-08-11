#if !defined(UPDATELORD_H)
#define UPDATELORD_H

#include "types.h"

void updateGameRunningParams(progParamsDataLord *progParamsData, logDataLord* logData);

void updateGameObjects(World *world, progParamsDataLord *progParamsData, worldParamsDataLord *worldParamsData, logDataLord *logData);

#endif