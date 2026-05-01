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
        if (e->coords.x - 1 >= 0 && map[(e->coords.x - 1) + mapSize.x  * e->coords.y].landType.canBeOccupied)
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

void restoreHungerEntity(World  *world, Entity *e, int foodOnMap, struct tm *tm, FILE *sourceLogFile)
{
    char* foodGameId = malloc(sizeof(char)*12);
    sprintf(foodGameId, "%d", e->targetFoodId);
    
    world->items[e->targetFoodId].number--;

    if (world->items[e->targetFoodId].number <= 0)
    {
        logToFile(sourceLogFile, tm, "Food with id |");
        rawLogToFile(sourceLogFile, foodGameId);
        rawLogToFile(sourceLogFile, "| is over\n");
        return;
    }

    e->hunger -= 10 + rand() % 10;
    if (e->hunger < 0) {
        e->hunger = 0;
    }

    free(foodGameId);
}

bool findNearestFood(World * world, Entity *e, int foodOnMap)
{
    int minDistance = 1000;
    bool isExistAnyFood = false;

    for (int z = 0; z < foodOnMap; z++) // finding nearest food
    {
        if (world->items[z].number > 0 && (abs(e->coords.x - world->items[z].coords.x) + abs(e->coords.y - world->items[z].coords.y)) < minDistance)
        {
            e->targetFoodId = z;
            minDistance = abs(e->coords.x - world->items[z].coords.x) + abs(e->coords.y - world->items[z].coords.y);

            e->targetCellCoords.x = world->items[z].coords.x;
            e->targetCellCoords.y = world->items[z].coords.y;

            isExistAnyFood = true;
        }
    }

    if (isExistAnyFood)
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;
}

void updateEntity(World *world, Coord mapSize, Entity *e, int timer, int foodOnMap, FILE *sourceLogFile, struct tm *tm) // updating entity
{
    int randomForMove = rand() % 5;

    bool isReachedTargetFood = (e->coords.x == world->items[e->targetFoodId].coords.x && e->coords.y == world->items[e->targetFoodId].coords.y && world->items[e->targetFoodId].number > 0);
    bool IsEnoughHungerReached = e->hunger >= (e->dieLevelHunger * 0.60);

    if (e->humanity == true)
    {
        // Select moving state
        // -------------------------------------------------------------------

        if (!IsEnoughHungerReached)
        {
            e->movingState = UNTARGET_MOVING; // boring
        }

        if (IsEnoughHungerReached && !isReachedTargetFood)
        {
            e->movingState = TARGETING;

            if (!findNearestFood(world, e, foodOnMap)) // find nearest food, if wasn't finded - boring
            {
                e->movingState = UNTARGET_MOVING;
                e->targetFoodId = -1;
            }
        }
        else if (IsEnoughHungerReached && isReachedTargetFood) // eating
        {
            e->movingState = EATING;
        }

        // Doing
        // -------------------------------------------------------------------

        if (timer % 10 == 0) // moving and eating
        {
            if (e->movingState == UNTARGET_MOVING) // random moving
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
                if (world->items[e->targetFoodId].number <= 0)
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
                    restoreHungerEntity(world, e, foodOnMap, tm, sourceLogFile);
                }
            }
        }

        // Updating hunger
        // -------------------------------------------------------------------

        if (timer % 1 == 0)
        {
            e->hunger += 0.01;
            e->sleepiness += 0.1;

            if (e->hunger >= e->dieLevelHunger) // die from hunger
            {
                e->isAlive = false;
                logToFile(sourceLogFile, tm, "Entity with id |");
                rawLogToFile(sourceLogFile, e->gameId);
                rawLogToFile(sourceLogFile, "| died from hunger\n");
            }
        }

        // Changing color
        // -------------------------------------------------------------------

        e->drawingColor = GREEN;

        if (world->map[e->coords.x + mapSize.x * e->coords.y].isSelected)
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
        } else if (IsEnoughHungerReached)
        {
            if (timer > 20) {
                e->drawingColor = BLUE;
            } else {
                e->drawingColor = GREEN;
            }
        }
    }
}
