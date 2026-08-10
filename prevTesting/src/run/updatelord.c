#include "updatelord.h"
#include "logging.h"
#include "entities.h"

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

void updateGameObjects(World *world, progParamsDataLord *progParamsData, worldParamsDataLord *worldParamsData, logDataLord *logData) { // Entities, items...
    for (int x = 0; x < worldParamsData->startEntitiesNumber; x++) // Update entities
    {
        if (world->entities[x].isAlive == true && !progParamsData->isPaused)
        {
            updateEntity(world, world->mapSize, &world->entities[x], progParamsData->timer, worldParamsData, logData);
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

void updateGameStats(World *world) { // Selected/alive entites, items...

}