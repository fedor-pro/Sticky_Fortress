#include "world.h"
#include "generate.h"
#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include "spawn.h"
#include "logging.h"

void initializeWorldLandscapes(World *world) 
{
    LandscapeType basicLandscape = {LAND_BASIC, "Basic landscape", true, (Color) {3, 130, 0, 225}};
    LandscapeType waterLandscape = {LAND_WATER, "Water surface", false, (Color){0, 0, 120, 255}};
    LandscapeType deepWaterLandscape = {LAND_DEEP_WATER, "Deep water", false, (Color){0, 0, 50, 255}};
    LandscapeType mountainsLandscape = {LAND_MOUNTAINS, "Mountains and hills", true, (Color){118, 114, 113, 255}};
    LandscapeType rockLandscape = {LAND_ROCK, "Hard rock landscape", false, (Color){68, 64, 63, 255}};

    world->worldLandscapes[0] = basicLandscape;
    world->worldLandscapes[1] = waterLandscape;
    world->worldLandscapes[2] = deepWaterLandscape;
    world->worldLandscapes[3] = mountainsLandscape;
    world->worldLandscapes[4] = rockLandscape;

    world->worldLandscapes[0] = (LandscapeType) {LAND_BASIC, "Basic landscape", true, (Color) {3, 130, 0, 225}};
}

void initializeWorldFile(World *world, progParamsDataLord *progParamsData, worldParamsDataLord *worldParamsData) {
    
    char *worldFilePath = malloc(progParamsData->textBufferSize);
    sprintf(worldFilePath, "./worlds/%s.txt", worldParamsData->defaultName);

    world->worldFile = fopen(worldFilePath, "w");

    fflush(world->worldFile);
}

void createWorldMap(World *world)
{
    for (int x = 0; x < world->mapSize.x; x++)
    {
        for (int y = 0; y < world->mapSize.y; y++)
        {
            LandscapeCell ld;

            ld.lCoord = (Coord){x, y};

            ld.isSelected = false;
            ld.isOccupied = false;

            ld.landType = world->worldLandscapes[0];

            world->map[x + world->mapSize.x * y] = ld;
        }
    }
}

void deselectAllWorldMap(World *world, worldParamsDataLord *worldParamsData) {
    for (int x = 0; x < world->mapSize.x; x++)
    {
        for (int y = 0; y < world->mapSize.y; y++)
        {
            world->map[x + world->mapSize.x * y].isSelected = false;
        }
    }

    for (int u = 0; u < 5; u++) // Reset selected landscape cells stats
    {
        worldParamsData->cellsSelected[u] = 0;
    }
}

void createDwarves(World *world, worldParamsDataLord *worldParamsData, progParamsDataLord *progParamsData, logDataLord *logData, drawDataLord *drawData)
{
    for (int x = 0; x < worldParamsData->startDwarvesNumber; x++)
    {
        int dwX = rand() % world->mapSize.x;
        int dwY = rand() % world->mapSize.y;

        while (!world->map[dwX + world->mapSize.x * dwY].landType.canBeOccupied)
        {
            dwX = rand() % world->mapSize.x;
            dwY = rand() % world->mapSize.y;
        }

        char *dwarfGameId = malloc(progParamsData->textBufferSize);
        sprintf(dwarfGameId, "%d", x);

        Dwarf dw = {dwarfGameId, "Dwarf", true, true, drawData->defaultHumanChar, dwX, dwY, UNTARGET_MOVING, -1, 0, 0, 0, 50 + rand () % 1000 + rand () % 50, 0, RED};
        //                              dwarvenness  is                                                      target target hunger die level sleepiness
        //                                           alive                                                  food id   cell coords    hunger   

        if (x > 0 && x < worldParamsData->startDwarvesNumber - 1) {

            if (x == 1) {
                rawLogToFile(logData, "...\n");
            }
        } else {
            logToFile(logData, "Created ");
            rawLogToFile(logData, dw.gameName);
            rawLogToFile(logData, " with id: |");
            rawLogToFile(logData, dw.gameId);
            rawLogToFile(logData,"|\n");
        }

        time(&logData->rawTime);
        logData->tm = localtime(&logData->rawTime); // Updating time

        spawnDwarf(world, (Coord) {dwX, dwY}, dw, x);
    }
}

void createWorldFood(World *world, worldParamsDataLord *worldParamsData, drawDataLord *drawData)
{
    for (int x = 0; x < worldParamsData->startFoodOnMap; x++)
    {
        int resX = rand() % world->mapSize.x;
        int resY = rand() % world->mapSize.y;

        while (!world->map[resX + world->mapSize.x * resY].landType.canBeOccupied)
        {
            resX = rand() % world->mapSize.x;
            resY = rand() % world->mapSize.y;
        }

        Item f = {resX, resY, FOOD, drawData->defaultFoodChar, 50 + rand() % 30, true};

        world->items[x] = f;
    }
}

void deleteWorld(World *world, worldParamsDataLord *worldParamsData, logDataLord *logData)
{
    fprintf(world->worldFile, "==DATA==\n");
    fprintf(world->worldFile, "[WORLD_NAME]:%s\n", world->worldName);
    fflush(world->worldFile);

    fprintf(world->worldFile, "[DWARVES_ALIVE]:%d\n", worldParamsData->dwarvesAlive);
    fprintf(world->worldFile, "[FOOD_REMAINING]:%d\n", worldParamsData->foodExists);
    fflush(world->worldFile);

    fprintf(world->worldFile, "\n==DWARVES_LIST==\n");

    for (int x = 0; x < worldParamsData->startDwarvesNumber; x++)
    {
        fprintf(world->worldFile, "{DWARF}[ID]:%s|[GAME_NAME]:%s|[DWARVENNESS]:%d|[IS_ALIVE]:%d|[DRAWING_CHAR]:%s|[COORDS]:%d,%d|[HUNGER]:%f|[DIE_LEVEL_HUNGER]:%f|[SLEEPINESS]:%f\n", 
            world->dwarves[x].gameId, world->dwarves[x].gameName, world->dwarves[x].dwarvenness, world->dwarves[x].isAlive, world->dwarves[x].charValue,
            world->dwarves[x].coords.x, world->dwarves[x].coords.y, world->dwarves[x].hunger, world->dwarves[x].dieLevelHunger, world->dwarves[x].sleepiness);

        fflush(world->worldFile);
    }

    fprintf(world->worldFile, "\n==WORLD_MAP==\n");

    for (int xx = 0; xx < world->mapSize.x; xx++)
    {
        for (int yy = 0; yy < world->mapSize.y; yy++)
        { 
            fprintf(world->worldFile, "{CELL}[COORDS]:%d,%d|[IS_SELECTED]:%d|[IS_OCCUPIED]:%d|[LANDSCAPE]:%d", 
                world->map[xx + world->mapSize.x * yy].lCoord.x, world->map[xx + world->mapSize.x * yy].lCoord.y, 
                world->map[xx + world->mapSize.x * yy].isSelected, world->map[xx + world->mapSize.x * yy].isOccupied, world->map[xx + world->mapSize.x * yy].landType);
            
            fflush(world->worldFile);
        }

        fprintf(world->worldFile, "\n");
    }

    for (int i = 0; i < worldParamsData->startDwarvesNumber; i++)
    {
        if (world->dwarves[i].gameId != NULL)
        {
            free(world->dwarves[i].gameId);
        }
    }

    free(world->map);
    free(world->worldLandscapes);
    free(world->dwarves);
    free(world->items);

    fclose(world->worldFile);
    free(world);
}

World *initializeWorld(worldParamsDataLord *worldParamsData, progParamsDataLord *progParamsData, char *logsBarriers, Coord mapSize, logDataLord *logData, drawDataLord *drawData)
{
    World *world = malloc(sizeof(World));
    world->worldName = worldParamsData->defaultName;
    world->mapSize = mapSize;
    world->map = malloc(sizeof(LandscapeCell) * (world->mapSize.x * world->mapSize.y)); // Creating map

    world->worldLandscapes = malloc(sizeof(LandscapeType)*10);
    initializeWorldLandscapes(world);

    initializeWorldFile(world, progParamsData, worldParamsData);

    createWorldMap(world);
    logToFile(logData, "INITIALIZED WORLD MAP\n");
    rawLogToFile(logData, logsBarriers);

    generateWorldStructures(world, worldParamsData);

    world->dwarves = malloc(sizeof(Dwarf) * (worldParamsData->startDwarvesNumber) * 1.5); // Creating dwarves

    createDwarves(world, worldParamsData, progParamsData, logData, drawData);

    rawLogToFile(logData, logsBarriers);

    world->items = malloc(sizeof(LandscapeCell) * (worldParamsData->startFoodOnMap + 5)); // Creating items

    createWorldFood(world, worldParamsData, drawData);

    // deselectAllWorldMap(world, worldParamsData);
    printf("%d\n", worldParamsData->cellsSelected[0]);

    return world;
}