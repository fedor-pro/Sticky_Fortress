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

    worldParamsData->startEntitiesNumber = 40;
    worldParamsData->startFoodOnMap = 10;
    worldParamsData->structuresNumber = 15;

    worldParamsData->entitiesAlive = worldParamsData->startEntitiesNumber;
    worldParamsData->entitiesSelected = 0;
    worldParamsData->foodExists = worldParamsData->startFoodOnMap;
}

drawDataLord* defineDrawDataLord () {
    drawDataLord* drawData = malloc(sizeof(drawDataLord));

    drawData->defaultFontSize = 23;
    drawData->defaultFoodChar = "*";
    drawData->defaultHumanChar = "&";
}