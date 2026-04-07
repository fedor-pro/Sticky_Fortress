#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma once
#include "types.h"
#include "entities.h"
#include "logging.h"
#include "generate.h"
#include "spawn.h"

void initializeWorldLandscapes(World *world) {
    LandscapeType basicLandscape = {LAND_BASIC, "Basic landscape", true, (Color){3, 130, 0, 225}};
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
    int i = 0;

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

void createEntities(World *world, int entitiesNumber, int textBufferSize, struct tm *tm, FILE  *sourceLogFile, time_t rawTime)
{
    for (int x = 0; x < entitiesNumber; x++)
    {
        int entX = rand() % world->mapSize.x;
        int entY = rand() % world->mapSize.y;

        while (!world->map[entX + world->mapSize.x * entY].landType.canBeOccupied)
        {
            entX = rand() % world->mapSize.x;
            entY = rand() % world->mapSize.y;
        }

        char *entityGameId = malloc(textBufferSize);
        sprintf(entityGameId, "%s%d", "entities:human", x);

        Entity ent = {entityGameId, "Human", true, true, "&", entX, entY, UNTARGET_MOVING, -1, 0, 0, 1, 50 + rand() % 10, 0, RED};
        //                             humanity   is                                target target hunger die level sleepiness
        //                                       alive                           food id   cell coords    hunger   
        
        logToFile(sourceLogFile, tm, "Created ");
        rawLogToFile(sourceLogFile, ent.gameName);
        rawLogToFile(sourceLogFile, " with id: ");
        rawLogToFile(sourceLogFile, ent.gameId);
        rawLogToFile(sourceLogFile ,"\n");

        time(&rawTime);
        tm = localtime(&rawTime); // updating time

        spawnEntity(world, (Coord) {entX, entY}, ent, x);
    }
}

void createWorldFood(World *world, int foodNumber)
{
    for (int x = 0; x < foodNumber; x++)
    {
        int resX = rand() % world->mapSize.x;
        int resY = rand() % world->mapSize.y;

        while (!world->map[resX + world->mapSize.x * resY].landType.canBeOccupied)
        {
            resX = rand() % world->mapSize.x;
            resY = rand() % world->mapSize.y;
        }

        Item f = {FOOD, "*", 30 + rand() % 20, true, resX, resY};

        world->items[x] = f;
    }
}

void deleteWorld(World *world, int entities_number)
{
    printf("Deleted world: %zu bytes\n", sizeof(world));

    for (int i = 0; i < entities_number; i++)
    {
        if (world->entities[i].gameId != NULL)
        {
            free(world->entities[i].gameId);
        }
    }

    free(world->map);
    free(world->entities);
    free(world->items);
    free(world);
}

// void updateWorld(World *world, int entities_list_size) {

//     for (int x = 0; x < entities_list_size; x++) // update and draw entities
//         {
//             if (world->entities[x].isAlive == true)
//             {
//                 updateEntity(world, world->mapSize, &world->entities[x], timer, FOOD_ON_MAP, sourceLogFile, tm);
//             }

//             drawEntity(world->entities[x], rectSizeX, rectSizeY);
//         }  
// }

World *initializeWorld(int structuresNumber, int textBufferSize, Coord mapSize, int foodOnMap, int entitiesNumber, struct tm *tm, FILE *source_log_file, time_t rawTime)
{
    World *world = malloc(sizeof(World));
    world->mapSize = mapSize;
    world->map = malloc(sizeof(LandscapeCell) * (world->mapSize.x * world->mapSize.y)); // creating map

    world->worldLandscapes = malloc(sizeof(LandscapeType)*10);
    initializeWorldLandscapes(world);

    createWorldMap(world);

    generateWorldStructures(world, structuresNumber);

    world->entities = malloc(sizeof(Entity) * (entitiesNumber) * 1.5); // creating entities

    createEntities(world, entitiesNumber, textBufferSize, tm, source_log_file, rawTime);

    world->items = malloc(sizeof(LandscapeCell) * (foodOnMap + 5)); // creating resources

    createWorldFood(world, foodOnMap);

    return world;
}