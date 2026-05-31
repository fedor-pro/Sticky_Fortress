#if !defined(DATALORDH)
#define DATALORD_H

#include "types.h"
#include "stdlib.h"

progParamsDataLord* defineProgParamsDataLord () {
    progParamsDataLord* progParamsData = malloc(sizeof(progParamsDataLord));

    progParamsData->windowSize = (Coord) {1900, 1000};
    progParamsData->rectSize = (Coord) {11, 11};
    progParamsData->textBufferSize = 500;
}

worldParamsDataLord* defineWorldParamsDataLord () {
    worldParamsDataLord* worldParamsData = malloc(sizeof(worldParamsDataLord));

    worldParamsData->entitiesNumber = 40;
    worldParamsData->foodOnMap = 10;
    worldParamsData->structuresNumber = 10;
}

#endif