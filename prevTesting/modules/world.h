#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma once
#include "types.h"
#include "entities.h"

void createWorldMap(World *world, Coord mapSize, LandscapeType basicLandscape)
{
    int i = 0;

    for (int x = 0; x < mapSize.x; x++)
    {
        for (int y = 0; y < mapSize.y; y++)
        {
            LandscapeCell ld;

            ld.lCoord = (Coord){x, y};

            ld.isSelected = false;
            ld.isOccupied = false;

            ld.landType = basicLandscape;

            world->map[x + mapSize.x * y] = ld;
        }
    }
}

void createEntities(World *world, int entitiesNumber, Coord mapSize, int textBufferSize)
{
    for (int x = 0; x < entitiesNumber; x++)
    {
        int entX = rand() % mapSize.x;
        int entY = rand() % mapSize.y;

        while (!world->map[entX + mapSize.x * entY].landType.canBeOccupied)
        {
            entX = rand() % mapSize.x;
            entY = rand() % mapSize.y;
        }

        char *entityGameId = malloc(textBufferSize);
        sprintf(entityGameId, "%s%d", "entities:human", x);

        Entity ent = {entityGameId, "Human", true, true, "&", entX, entY, UNTARGET_MOVING, -1, 0, 0, 0, 50 + rand() % 10, 0, RED};
        //                             humanity   is                                target target hunger die level sleepiness
        //                                       alive                           food id   cell coords    hunger                    

        world->map[entX + mapSize.x * entY].isOccupied = true;

        world->entities[x] = ent;
    }
}

void createWorldFood(World *world, int foodNumber, Coord mapSize)
{
    for (int x = 0; x < foodNumber; x++)
    {
        int resX = rand() % mapSize.x;
        int resY = rand() % mapSize.y;

        while (!world->map[resX + mapSize.x * resY].landType.canBeOccupied)
        {
            resX = rand() % mapSize.x;
            resY = rand() % mapSize.y;
        }

        Item f = {FOOD, "o", 30 + rand() % 20, true, resX, resY};

        world->items[x] = f;
    }
}

void deleteWorld(World *world, int entities_number)
{
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

World *initializeWorld(int textBufferSize, Coord mapSize, int foodOnMap, int entitiesNumber, LandscapeType basicLandscape)
{
    World *world = malloc(sizeof(World));
    world->mapSize = mapSize;
    world->map = malloc(sizeof(LandscapeCell) * (world->mapSize.x * world->mapSize.y)); // creating map

    createWorldMap(world, mapSize, basicLandscape);

    world->entities = malloc(sizeof(Entity) * (entitiesNumber) * 1.5); // creating entities

    createEntities(world, entitiesNumber, mapSize, textBufferSize);

    world->items = malloc(sizeof(LandscapeCell) * (foodOnMap + 5)); // creating resources

    createWorldFood(world, foodOnMap, mapSize);

    return world;
}