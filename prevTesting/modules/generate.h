// STANDALONE MODULE

#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma once
#include "types.h"

void generateStructure(Coord mapSize, LandscapeCell * map, LandscapeType basicLandscape, LandscapeType waterLandscape, LandscapeType deepWaterLandscape, LandscapeType mountainsLandscape, LandscapeType rockLandscape)
{
    LandscapeStructure structure;
    structure.incomingCellsCoords = malloc(2048);

    float pond = 0.70;

    if (rand() % 10 > 2)
    {
        structure.landscape = waterLandscape;
    }
    else
    {
        structure.landscape = mountainsLandscape;
    }

    Coord zoneStartCoord = (Coord){rand() % mapSize.x, rand() % mapSize.y};

    int zoneMaxLongX = 3 + rand() % 5;
    int zoneMaxLongY = 3 + rand() % 5;

    int j = 0;

    while (!((zoneStartCoord.x + zoneMaxLongX) < mapSize.x && (zoneStartCoord.y + zoneMaxLongY) < mapSize.y))
    {
        zoneStartCoord.x = 2 + rand() % 5;
        zoneStartCoord.y = 2 + rand() % 5;
    }

    for (int l = 0; l < zoneMaxLongX; l++)
    {
        for (int g = 0; g < zoneMaxLongY; g++)
        {
            if (!(structure.landscape.gameId == LAND_MOUNTAINS))
            {
                structure.incomingCellsCoords[j].x = zoneStartCoord.x + l;
                structure.incomingCellsCoords[j].y = zoneStartCoord.y + g;

                map[(zoneStartCoord.x + l) + mapSize.x * (zoneStartCoord.y + g)].landType = structure.landscape;

                j++;
            }
            else
            {
                if ((rand() % 100) > pond)
                {
                    structure.incomingCellsCoords[j].x = zoneStartCoord.x + l;
                    structure.incomingCellsCoords[j].y = zoneStartCoord.y + g;

                    map[(zoneStartCoord.x + l) + mapSize.x * (zoneStartCoord.y + g)].landType = structure.landscape;

                    j++;
                }
            }

            pond += 2;
        }
    }

    free(structure.incomingCellsCoords);
}

void generateWorldStructures(int numberOfStructures, Coord mapSize, LandscapeCell * map, LandscapeType basicLandscape, LandscapeType waterLandscape, LandscapeType deepWaterLandscape, LandscapeType mountainsLandscape, LandscapeType rockLandscape)
{
    for (int x = 0; x < numberOfStructures; x++)
    {
        generateStructure(mapSize, map, basicLandscape, waterLandscape, deepWaterLandscape, mountainsLandscape, rockLandscape);
    }

    return;
} // Generate structures