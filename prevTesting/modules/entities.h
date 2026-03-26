#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma once
#include "types.h"

void moveEntity(Direction direct, Entity *e, Coord mapSize, LandscapeCell *map)
{
    switch (direct)
    {
    case RIGHT:
        if (e->coords.x + 1 < mapSize.x && map[(e->coords.x + 1) + mapSize.x * e->coords.y].landType.canBeOccupied)
        {
            map[e->coords.x + mapSize.x * e->coords.y].isOccupied = false;

            e->coords.x++;

            map[e->coords.x + mapSize.x * e->coords.y].isOccupied = true;
        }

        break;
    case LEFT:
        if (e->coords.x - 1 >= 0 && map[e->coords.x + (mapSize.x - 1) * e->coords.y].landType.canBeOccupied)
        {
            map[e->coords.x + mapSize.x * e->coords.y].isOccupied = false;

            e->coords.x--;

            map[e->coords.x + mapSize.x * e->coords.y].isOccupied = true;
        }

        break;
    case DOWN:
        if (e->coords.y + 1 < mapSize.y && map[e->coords.x + mapSize.x * (e->coords.y + 1)].landType.canBeOccupied)
        {
            map[e->coords.x + mapSize.x * e->coords.y].isOccupied = false;

            e->coords.y++;

            map[e->coords.x + mapSize.x * e->coords.y].isOccupied = true;
        }

        break;
    case UP:
        if (e->coords.y - 1 >= 0 && map[e->coords.x + mapSize.x * (e->coords.y - 1)].landType.canBeOccupied)
        {
            map[e->coords.x + mapSize.x * e->coords.y].isOccupied = false;
            e->coords.y--;

            map[e->coords.x + mapSize.x * e->coords.y].isOccupied = true;
        }

        break;
    }
}

void restoreHungerEntity(World  *world, Entity *e, int foodOnMap)
{
    if (world->resources[e->targetFoodId].number <= 0)
    {
        return;
    }
    world->resources[e->targetFoodId].number--;

    if (e->hunger - 1 > 0)
    {
        e->hunger--;
    }
}

bool findNearestFood(World * world, Entity *e, int foodOnMap)
{
    // int minDistance = 1000;
    // bool isExistAnyFood = false;

    // for (int z = 0; z < foodOnMap; z++) // finding nearest food
    // {
    //     if (foodList[z].number > 0 && (abs(e->coords.x - [z].coords.x) + abs(e->coords.y - foodList[z].coords.y)) < minDistance)
    //     {
    //         e->targetFoodId = z;
    //         minDistance = abs(e->coords.x - foodList[z].coords.x) + abs(e->coords.y - foodList[z].coords.y);

    //         e->targetCellCoords.x = foodList[z].coords.x;
    //         e->targetCellCoords.y = foodList[z].coords.y;

    //         isExistAnyFood = true;
    //     }
    // }

    // if (isExistAnyFood)
    // {
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }

    return false;
}

void updateEntity(World *world, Coord mapSize, Entity *e, int timer, int foodOnMap, FILE *sourceLogFile, struct tm *tm) // updating entity
{
    int randomForMove = rand() % 5;

    // bool isReachedTargetFood = (e->coords.x == foodList[e->targetFoodId].coords.x && e->coords.y == foodList[e->targetFoodId].coords.y && foodList[e->targetFoodId].number > 0);
    bool IsEnoughHungerReached = e->hunger >= (e->dieLevelHunger * 0.40);

    if (e->humanity == true)
    {
        // if (IsEnoughHungerReached && !isReachedTargetFood) // select moving state
        // {
        //     e->movingState = TARGETING;

        //     if (!findNearestFood(e, foodOnMap, foodList))
        //     {
        //         e->movingState = UNTARGET_MOVING;
        //         e->targetFoodId = -1;
        //     }
        // }
        // else if (IsEnoughHungerReached && isReachedTargetFood)
        // {

        //     e->movingState = EATING;
        // }
        if (!IsEnoughHungerReached)
        {
            e->movingState = UNTARGET_MOVING;
        }

        if (timer % 5 == 0) // moving and eating
        {
            if (e->movingState == UNTARGET_MOVING) // boring
            {
                switch (randomForMove) {
                    case 1:
                        moveEntity(RIGHT, e, mapSize, world->map);
                    break;
                    case 2:
                        moveEntity(LEFT, e, mapSize, world->map);
                    break;
                    case 3:
                        moveEntity(DOWN, e, mapSize, world->map);
                    break;
                    case 4:
                        moveEntity(UP, e, mapSize, world->map);
                    break;
                }

            }
            else if (e->movingState == TARGETING) // go to target
            {
                if (e->coords.x < e->targetCellCoords.x)
                {
                    moveEntity(RIGHT, e, mapSize, world->map);
                }
                else if (e->coords.x > e->targetCellCoords.x)
                {
                    moveEntity(LEFT, e, mapSize, world->map);
                }

                else if (e->coords.y < e->targetCellCoords.y)
                {
                    moveEntity(DOWN, e, mapSize, world->map);
                }
                else if (e->coords.y > e->targetCellCoords.y)
                {
                    moveEntity(UP, e, mapSize, world->map);
                }
            }
            else if (e->movingState == EATING) // eating
            {
                if (world->resources[e->targetFoodId].number <= 0)
                {
                    e->movingState = TARGETING;

                    if (!findNearestFood(world, e, foodOnMap))
                    {
                        e->movingState = UNTARGET_MOVING;
                        e->targetFoodId = -1;
                    }
                }
                else
                {
                    restoreHungerEntity(world, e, foodOnMap);
                }
            }
        }

        if (timer % 1 == 0) // updating hunger
        {
            e->hunger += 0.1;
            e->sleepiness += 0.1;

            if (e->hunger >= e->dieLevelHunger) // die from hunger
            {
                e->isAlive = false;
                logToFile(sourceLogFile, tm, "Entity with id |");
                rawLogToFile(sourceLogFile, e->gameId);
                rawLogToFile(sourceLogFile, "| died from hunger\n");
                printf("%s\n", e->gameId);
            }
        }

        if (world->map[e->coords.x + mapSize.x * e->coords.y].isSelected) // color
        {
            e->drawingColor = RED;
        }
        else if (!e->isAlive)
        {
            e->drawingColor = GRAY;
        }
        else if (e->movingState == EATING)
        {
            e->drawingColor = ORANGE;
        }
        else
        {
            e->drawingColor = GREEN;
        }
    }
}
