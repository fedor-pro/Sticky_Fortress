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

void deselectAllWorldMap(World *world) {
    for (int x = 0; x < world->mapSize.x; x++)
    {
        for (int y = 0; y < world->mapSize.y; y++)
        {
            world->map[x + world->mapSize.x * y].isSelected = false;
        }
    }
}

void createEntities(World *world, worldParamsDataLord* worldParamsData, progParamsDataLord* progParamsData, logDataLord* logData, drawDataLord *drawData)
{
    for (int x = 0; x < worldParamsData->startEntitiesNumber; x++)
    {
        int entX = rand() % world->mapSize.x;
        int entY = rand() % world->mapSize.y;

        while (!world->map[entX + world->mapSize.x * entY].landType.canBeOccupied)
        {
            entX = rand() % world->mapSize.x;
            entY = rand() % world->mapSize.y;
        }

        char *entityGameId = malloc(progParamsData->textBufferSize);
        sprintf(entityGameId, "%d", x);

        Entity ent = {entityGameId, "Human", true, true, drawData->defaultHumanChar, entX, entY, UNTARGET_MOVING, -1, 0, 0, 0, 50 + rand () % 1000 + rand () % 50, 0, RED};
        //                             humanity   is                                target target hunger die level sleepiness
        //                                       alive                           food id   cell coords    hunger   

        if (x > 0 && x < worldParamsData->startEntitiesNumber - 1) {

            if (x == 1) {
                rawLogToFile(logData, "...\n");
            }
        } else {
            logToFile(logData, "Created ");
            rawLogToFile(logData, ent.gameName);
            rawLogToFile(logData, " with id: |");
            rawLogToFile(logData, ent.gameId);
            rawLogToFile(logData,"|\n");
        }

        time(&logData->rawTime);
        logData->tm = localtime(&logData->rawTime); // updating time

        spawnEntity(world, (Coord) {entX, entY}, ent, x);
    }
}

void createWorldFood(World *world, worldParamsDataLord* worldParamsData, drawDataLord* drawData)
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

    fprintf(world->worldFile, "[ENTITIES_ALIVE]:%d\n", worldParamsData->entitiesAlive);
    fprintf(world->worldFile, "[FOOD_REMAINING]:%d\n", worldParamsData->foodExists);
    fflush(world->worldFile);

    fprintf(world->worldFile, "\n==ENTITIES_LIST==\n");

    for (int x = 0; x < worldParamsData->startEntitiesNumber; x++)
    {
        fprintf(world->worldFile, "{ENTITY}[ID]:%s|[GAME_NAME]:%s|[HUMANITY]:%d|[IS_ALIVE]:%d|[DRAWING_CHAR]:%s|[COORDS]:%d,%d|[HUNGER]:%f|[DIE_LEVEL_HUNGER]:%f|[SLEEPINESS]:%f\n", 
            world->entities[x].gameId, world->entities[x].gameName, world->entities[x].humanity, world->entities[x].isAlive, world->entities[x].charValue,
            world->entities[x].coords.x, world->entities[x].coords.y, world->entities[x].hunger, world->entities[x].dieLevelHunger, world->entities[x].sleepiness);

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

    for (int i = 0; i < worldParamsData->startEntitiesNumber; i++)
    {
        if (world->entities[i].gameId != NULL)
        {
            free(world->entities[i].gameId);
        }
    }

    free(world->map);
    free(world->worldLandscapes);
    free(world->entities);
    free(world->items);

    fclose(world->worldFile);
    free(world);
}

World *initializeWorld(worldParamsDataLord* worldParamsData, progParamsDataLord* progParamsData, char* logsBarriers, Coord mapSize, logDataLord* logData, drawDataLord* drawData)
{
    World *world = malloc(sizeof(World));
    world->worldName = worldParamsData->defaultName;
    world->mapSize = mapSize;
    world->map = malloc(sizeof(LandscapeCell) * (world->mapSize.x * world->mapSize.y)); // creating map

    world->worldLandscapes = malloc(sizeof(LandscapeType)*10);
    initializeWorldLandscapes(world);

    initializeWorldFile(world, progParamsData, worldParamsData);

    createWorldMap(world);
    logToFile(logData, "INITIALIZED WORLD MAP\n");
    rawLogToFile(logData, logsBarriers);

    generateWorldStructures(world, worldParamsData);

    world->entities = malloc(sizeof(Entity) * (worldParamsData->startEntitiesNumber) * 1.5); // creating entities

    createEntities(world, worldParamsData, progParamsData, logData, drawData);

    rawLogToFile(logData, logsBarriers);

    world->items = malloc(sizeof(LandscapeCell) * (worldParamsData->startFoodOnMap + 5)); // creating resources

    createWorldFood(world, worldParamsData, drawData);

    return world;
}