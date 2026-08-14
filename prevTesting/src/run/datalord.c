#include "types.h"
#include <stdlib.h>
#include <time.h>

progParamsDataLord *defineProgParamsDataLord () {
    progParamsDataLord *progParamsData = malloc(sizeof(progParamsDataLord));

    progParamsData->windowSize = (Coord) {1900, 1000};
    progParamsData->rectSize = (Coord) {15, 15};
    progParamsData->textBufferSize = 500;

    progParamsData->timer = 0;
    progParamsData->currentFPS = 0;
    progParamsData->stringCurrentFPS = malloc(progParamsData->textBufferSize);
    progParamsData->isPaused = false;

    progParamsData->timerReset = 60;

    return progParamsData;
}

worldParamsDataLord *defineWorldParamsDataLord () {
    worldParamsDataLord *worldParamsData = malloc(sizeof(worldParamsDataLord));

    worldParamsData->defaultName = "test_world";

    worldParamsData->startDwarvesNumber = 100;
    worldParamsData->startFoodOnMap = 15;
    worldParamsData->structuresNumber = 15;

    worldParamsData->dwarvesAlive = worldParamsData->startDwarvesNumber;
    worldParamsData->dwarvesSelected = 0;
    worldParamsData->foodExists = worldParamsData->startFoodOnMap;

    worldParamsData->cellsSelected = malloc(sizeof(int)*5);

    return worldParamsData;
}

drawDataLord *defineDrawDataLord () {
    drawDataLord *drawData = malloc(sizeof(drawDataLord));

    drawData->defaultFontSize = 23;
    drawData->defaultFoodChar = "*";
    drawData->defaultHumanChar = "&";

    return drawData;
}

logDataLord *defineLogDataLord () {
    logDataLord *logData = malloc(sizeof(logDataLord));

    logData->rawTime = time(NULL);
    logData->tm = localtime(&(logData->rawTime));
    time(&logData->rawTime);

    // logData->sourceLogFile will be initialize in logging.c    

    return logData;
}

void undefineAllDatalords (progParamsDataLord *progParamsData, worldParamsDataLord *worldParamsData, drawDataLord *drawData, logDataLord *logData) {
    free(progParamsData);
    free(worldParamsData->cellsSelected);
    free(worldParamsData);
    free(drawData);
    free(logData);
}