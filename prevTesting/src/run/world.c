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
    LandscapeType waterLandscape = {LAND_WATER, "Water surface", false, (Color){15, 20, 65, 255}};
    LandscapeType deepWaterLandscape = {LAND_DEEP_WATER, "Deep water", false, (Color){0, 0, 139, 255}};
    LandscapeType mountainsLandscape = {LAND_MOUNTAINS, "Mountains and hills", true, (Color){128, 124, 123, 255}};
    LandscapeType rockLandscape = {LAND_ROCK, "Hard rock landscape", false, (Color){78, 74, 73, 255}};

    world->worldLandscapes[0] = basicLandscape;
    world->worldLandscapes[1] = waterLandscape;
    world->worldLandscapes[2] = deepWaterLandscape;
    world->worldLandscapes[3] = mountainsLandscape;
    world->worldLandscapes[4] = rockLandscape;
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

void createEntities(World *world, worldParamsDataLord* worldParamsData, progParamsDataLord* progParamsData, struct tm *tm, FILE  *sourceLogFile, time_t rawTime)
{
    for (int x = 0; x < worldParamsData->entitiesNumber; x++)
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

        Entity ent = {entityGameId, "Human", true, true, "&", entX, entY, UNTARGET_MOVING, -1, 0, 0, 0, rand () % 100 + rand () % 50, 0, RED};
        //                             humanity   is                                target target hunger die level sleepiness
        //                                       alive                           food id   cell coords    hunger   
        
        if (x > 0 && x < worldParamsData->entitiesNumber - 1) {
            if (x == 1) {
                rawLogToFile(sourceLogFile, "...\n");
            }
        } else {
            logToFile(sourceLogFile, tm, "Created ");
            rawLogToFile(sourceLogFile, ent.gameName);
            rawLogToFile(sourceLogFile, " with id: |");
            rawLogToFile(sourceLogFile, ent.gameId);
            rawLogToFile(sourceLogFile ,"|\n");
        }

        time(&rawTime);
        tm = localtime(&rawTime); // updating time

        spawnEntity(world, (Coord) {entX, entY}, ent, x);
    }
}

void createWorldFood(World *world, worldParamsDataLord* worldParamsData)
{
    for (int x = 0; x < worldParamsData->foodOnMap; x++)
    {
        int resX = rand() % world->mapSize.x;
        int resY = rand() % world->mapSize.y;

        while (!world->map[resX + world->mapSize.x * resY].landType.canBeOccupied)
        {
            resX = rand() % world->mapSize.x;
            resY = rand() % world->mapSize.y;
        }

        Item f = {resX, resY, FOOD, "*", 50 + rand() % 30, true};

        world->items[x] = f;
    }
}

void deleteWorld(World *world, worldParamsDataLord* worldParamsData)
{
    printf("̶D̶e̶l̶e̶t̶e̶d̶ ̶w̶o̶r̶l̶d: ̶%zu̶ ̶b̶y̶t̶e̶s̶\n", sizeof(world));

    for (int i = 0; i < worldParamsData->entitiesNumber; i++)
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
    free(world);
}

World *initializeWorld(worldParamsDataLord* worldParamsData, progParamsDataLord* progParamsData, char* logsBarriers, Coord mapSize, struct tm *tm, FILE *sourceLogFile, time_t rawTime)
{
    World *world = malloc(sizeof(World));
    world->mapSize = mapSize;
    world->map = malloc(sizeof(LandscapeCell) * (world->mapSize.x * world->mapSize.y)); // creating map

    world->worldLandscapes = malloc(sizeof(LandscapeType)*10);
    initializeWorldLandscapes(world);

    createWorldMap(world);
    logToFile(sourceLogFile, tm, "INITIALIZED WORLD MAP\n");
    rawLogToFile(sourceLogFile, logsBarriers);

    generateWorldStructures(world, worldParamsData);

    world->entities = malloc(sizeof(Entity) * (worldParamsData->entitiesNumber) * 1.5); // creating entities

    createEntities(world, worldParamsData, progParamsData, tm, sourceLogFile, rawTime);
    rawLogToFile(sourceLogFile, logsBarriers);

    world->items = malloc(sizeof(LandscapeCell) * (worldParamsData->foodOnMap + 5)); // creating resources

    createWorldFood(world, worldParamsData);

    return world;
}