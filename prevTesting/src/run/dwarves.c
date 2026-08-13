#include "dwarves.h"
#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include "logging.h"

void moveDwarf(Direction direct, Dwarf *dw, Coord mapSize, LandscapeCell *map)
{
    // Aaaaaaaargh
    // TODO: Fix all

    switch (direct)
    {
    case RIGHT:
        if (dw->coords.x + 1 < mapSize.x && map[(dw->coords.x + 1) + mapSize.x * dw->coords.y].landType.canBeOccupied)
        {
            map[dw->coords.x + mapSize.x * dw->coords.y].isOccupied = false;
            dw->coords.x++;

            map[dw->coords.x + mapSize.x * dw->coords.y].isOccupied = true;
        }

        break;
    case LEFT:
        if (dw->coords.x - 1 >= 0 && map[(dw->coords.x - 1) + mapSize.x  * dw->coords.y].landType.canBeOccupied)
        {
            map[dw->coords.x + mapSize.x * dw->coords.y].isOccupied = false;
            dw->coords.x--;

            map[dw->coords.x + mapSize.x * dw->coords.y].isOccupied = true;
        }

        break;
    case DOWN:
        if (dw->coords.y + 1 < mapSize.y && map[dw->coords.x + mapSize.x * (dw->coords.y + 1)].landType.canBeOccupied)
        {
            map[dw->coords.x + mapSize.x * dw->coords.y].isOccupied = false;
            dw->coords.y++;

            map[dw->coords.x + mapSize.x * dw->coords.y].isOccupied = true;
        }

        break;
    case UP:
        if (dw->coords.y - 1 >= 0 && map[dw->coords.x + mapSize.x * (dw->coords.y - 1)].landType.canBeOccupied)
        {
            map[dw->coords.x + mapSize.x * dw->coords.y].isOccupied = false;
            dw->coords.y--;

            map[dw->coords.x + mapSize.x * dw->coords.y].isOccupied = true;
        }

        break;
    }
}

void restoreHungerDwarf(World  *world, Dwarf *dw, worldParamsDataLord *worldParamsData, logDataLord *logData)
{
    if (world->items[dw->targetFoodId].number > 0)
    {        
        world->items[dw->targetFoodId].number--;

        dw->hunger -= 10 + rand() % 10;
        if (dw->hunger < 0) {
            dw->hunger = 0;
        }
    }   
}

bool findNearestFood(World *world, Dwarf *dw, worldParamsDataLord *worldParamsData)
{
    int minDistance = 1000;
    bool isExistAnyFood = false;

    for (int z = 0; z < worldParamsData->startFoodOnMap; z++) // Finding nearest food
    {
        if (world->items[z].number > 0 && (abs(dw->coords.x - world->items[z].coords.x) + abs(dw->coords.y - world->items[z].coords.y)) < minDistance)
        {
            dw->targetFoodId = z;
            minDistance = abs(dw->coords.x - world->items[z].coords.x) + abs(dw->coords.y - world->items[z].coords.y);

            dw->targetCellCoords.x = world->items[z].coords.x;
            dw->targetCellCoords.y = world->items[z].coords.y;

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

void updateDwarf(World *world, Coord mapSize, Dwarf *dw, int timer, worldParamsDataLord *worldParamsData, logDataLord *logData) // Updating dwarf
{
    int randomForMove = rand() % 5;

    bool isReachedTargetFood = (dw->coords.x == world->items[dw->targetFoodId].coords.x && dw->coords.y == world->items[dw->targetFoodId].coords.y && world->items[dw->targetFoodId].number > 0);
    bool IsEnoughHungerReached = dw->hunger >= (dw->dieLevelHunger * 0.60);

    if (dw->dwarvenness == true)
    {
        if (!IsEnoughHungerReached)
        {
            dw->movingState = UNTARGET_MOVING; // Boring
        }

        if (IsEnoughHungerReached && !isReachedTargetFood)
        {
            dw->movingState = TARGETING;

            if (!findNearestFood(world, dw, worldParamsData)) // Find nearest food, if wasn't finded - boring
            {
                dw->movingState = UNTARGET_MOVING;
                dw->targetFoodId = -1;
            }
        }
        else if (IsEnoughHungerReached && isReachedTargetFood) // Eating
        {
            dw->movingState = EATING;
        }

        if (timer % 10 == 0) // Moving and eating
        {
            if (dw->movingState == UNTARGET_MOVING) // Random moving
            {
                switch (randomForMove) {
                    case 1:
                        moveDwarf(RIGHT, dw, mapSize, world->map);
                    break;
                    case 2:
                        moveDwarf(LEFT, dw, mapSize, world->map);
                    break;
                    case 3:
                        moveDwarf(DOWN, dw, mapSize, world->map);
                    break;
                    case 4:
                        moveDwarf(UP, dw, mapSize, world->map);
                    break;
                }

            }
            else if (dw->movingState == TARGETING) // Go to target
            {
                if (dw->coords.x < dw->targetCellCoords.x)
                {
                    moveDwarf(RIGHT, dw, mapSize, world->map);
                }
                else if (dw->coords.x > dw->targetCellCoords.x)
                {
                    moveDwarf(LEFT, dw, mapSize, world->map);
                }

                else if (dw->coords.y < dw->targetCellCoords.y)
                {
                    moveDwarf(DOWN, dw, mapSize, world->map);
                }
                else if (dw->coords.y > dw->targetCellCoords.y)
                {
                    moveDwarf(UP, dw, mapSize, world->map);
                }
            }
            else if (dw->movingState == EATING) // Eating
            {
                if (world->items[dw->targetFoodId].number <= 0)
                {
                    dw->movingState = TARGETING;

                    if (!findNearestFood(world, dw, worldParamsData))
                    {
                        dw->movingState = UNTARGET_MOVING;
                        dw->targetFoodId = -1;
                    }
                }
                else
                {
                    restoreHungerDwarf(world, dw, worldParamsData, logData);
                }
            }
        }

        if (timer % 1 == 0)
        {
            dw->hunger += 0.001;
            if (rand() % 2 > 0) {
                dw->hunger += 0.0005;
            }

            if (dw->hunger >= dw->dieLevelHunger) // Die from hunger
            {
                dw->isAlive = false;
                logToFile(logData, "Dwarf with id |");
                rawLogToFile(logData, dw->gameId);
                rawLogToFile(logData, "| died from hunger\n");
            }
        }

        dw->drawingColor = GREEN;

        if (world->map[dw->coords.x + mapSize.x * dw->coords.y].isSelected)
        {
            dw->drawingColor = RED;
            worldParamsData->dwarvesSelected ++;
        }
        else if (!dw->isAlive)
        {
            dw->drawingColor = GRAY;
            worldParamsData->dwarvesAlive ++;
        }
        else if (dw->movingState == EATING)
        {
            dw->drawingColor = ORANGE;
        } else if (IsEnoughHungerReached)
        {
            if (timer > 20) {
                dw->drawingColor = BLUE;
            } else {
                dw->drawingColor = GREEN;
            }
        }
    }
}