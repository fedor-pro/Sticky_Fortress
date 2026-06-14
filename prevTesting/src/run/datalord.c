#include "types.h"
#include <stdlib.h>
#include <time.h>

progParamsDataLord* defineProgParamsDataLord () {
    progParamsDataLord* progParamsData = malloc(sizeof(progParamsDataLord));

    progParamsData->windowSize = (Coord) {1900, 1000};
    progParamsData->rectSize = (Coord) {11, 11};
    progParamsData->textBufferSize = 500;

    return progParamsData;
}

worldParamsDataLord* defineWorldParamsDataLord () {
    worldParamsDataLord* worldParamsData = malloc(sizeof(worldParamsDataLord));

    worldParamsData->defaultName = "test_world";

    worldParamsData->startEntitiesNumber = 40;
    worldParamsData->startFoodOnMap = 10;
    worldParamsData->structuresNumber = 15;

    worldParamsData->entitiesAlive = worldParamsData->startEntitiesNumber;
    worldParamsData->entitiesSelected = 0;
    worldParamsData->foodExists = worldParamsData->startFoodOnMap;

    return worldParamsData;
}

drawDataLord* defineDrawDataLord () {
    drawDataLord* drawData = malloc(sizeof(drawDataLord));

    drawData->defaultFontSize = 23;
    drawData->defaultFoodChar = "*";
    drawData->defaultHumanChar = "&";

    return drawData;
}

logDataLord* defineLogDataLord () {
    logDataLord* logData = malloc(sizeof(logDataLord));

    logData->rawTime = time(NULL);
    logData->tm = localtime(&(logData->rawTime));
    time(&logData->rawTime);

    // logData->sourceLogFile will be initialize in logging.c    

    return logData;
}