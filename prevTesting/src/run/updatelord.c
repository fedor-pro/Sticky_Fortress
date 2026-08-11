#include "updatelord.h"
#include "logging.h"
#include "dwarves.h"

void updateGameRunningParams(progParamsDataLord *progParamsData, logDataLord *logData) { // Timer, fps, pause, time...
    time(&logData->rawTime);
    logData->tm = localtime(&logData->rawTime);

    progParamsData->currentFPS = GetFPS();

    if (IsKeyPressed(KEY_SPACE)) // Pause
    {
        progParamsData->isPaused = !progParamsData->isPaused;
    }

    if (!progParamsData->isPaused)
    {
        progParamsData->timer++; // Update timer
    }
}

void updateGameObjects(World *world, progParamsDataLord *progParamsData, worldParamsDataLord *worldParamsData, logDataLord *logData) { // Dwarves, items...
    for (int x = 0; x < worldParamsData->startDwarvesNumber; x++) // Update dwarves
    {
        if (world->dwarves[x].isAlive == true && !progParamsData->isPaused)
        {
            updateDwarf(world, world->mapSize, &world->dwarves[x], progParamsData->timer, worldParamsData, logData);
        }
    } 
    
    if (!progParamsData->isPaused && progParamsData->timer >= progParamsData->timerReset) // Update items
    {
        progParamsData->timer = 0;
        
        for (int u = 0; u < worldParamsData->startFoodOnMap; u ++) {
            if ((rand() % 100) == 50) {
                world->items[u].number ++;

                char *si = malloc(sizeof(char)*12);
                sprintf(si, "%d", u);

                logToFile(logData, "Food with id |");
                rawLogToFile(logData, si);
                rawLogToFile(logData, "| restored\n");
            }
        }

        sprintf(progParamsData->stringCurrentFPS, "%d", progParamsData->currentFPS);
        logToFile(logData, "Current FPS: ");
        rawLogToFile(logData, progParamsData->stringCurrentFPS);
        rawLogToFile(logData, "\n");
    }
}