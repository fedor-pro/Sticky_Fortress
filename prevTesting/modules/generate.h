// STANDALONE MODULE

#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma once
#include "types.h"

bool structuresOverlayCheck(World *world, LandscapeStructure structure, Coord zoneStartCoord, int ZoneMaxLongX, int zoneMaxLongY, int l, int g, LandscapeType basicLandscape) {
    if (world->map[(zoneStartCoord.x + l) + world->mapSize.x * (zoneStartCoord.y + g)].landType.gameId != basicLandscape.gameId) { // Overlay check
        free(structure.incomingCellsCoords);
        return true;
    }

    return false;
}

void generateStructure(World *world, LandscapeType basicLandscape, LandscapeType waterLandscape, LandscapeType deepWaterLandscape, LandscapeType mountainsLandscape, LandscapeType rockLandscape)
{
    LandscapeStructure structure;

    float pond = 74;
    float pondIncrementStep = 2;
    int pondBorder = 200 + rand() % 100;

    if (rand() % 10 > 5)
    {
        structure.landscape = waterLandscape;
    }
    else
    {
        structure.landscape = mountainsLandscape;
    }

    Coord zoneStartCoord = (Coord){rand() % world->mapSize.x, rand() % world->mapSize.y};

    int zoneMaxLongX = 3 + rand() % 20;
    int zoneMaxLongY = 3 + rand() % 10;

    structure.incomingCellsCoords = malloc(sizeof(LandscapeCell) * zoneMaxLongX * zoneMaxLongY);

    int j = 0;

    while (!((zoneStartCoord.x + zoneMaxLongX) < world->mapSize.x && (zoneStartCoord.y + zoneMaxLongY) < world->mapSize.y))
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

                if (structuresOverlayCheck(world, structure, zoneStartCoord, zoneMaxLongX, zoneMaxLongY, l, g, basicLandscape)) {
                    return;
                }

                world->map[(zoneStartCoord.x + l) + world->mapSize.x * (zoneStartCoord.y + g)].landType = structure.landscape;

                j++;
            }
            else
            {
                if ((rand() % pondBorder) > pond)
                {
                    structure.incomingCellsCoords[j].x = zoneStartCoord.x + l;
                    structure.incomingCellsCoords[j].y = zoneStartCoord.y + g;

                    if (structuresOverlayCheck(world, structure, zoneStartCoord, zoneMaxLongX, zoneMaxLongY, l, g, basicLandscape)) {
                        return;
                    }

                    world->map[(zoneStartCoord.x + l) + world->mapSize.x * (zoneStartCoord.y + g)].landType = structure.landscape;

                    j++;
                }
            }

            pond += pondIncrementStep;
        }
    }

    free(structure.incomingCellsCoords);
}

void generateWorldStructures(World *world, int numberOfStructures, LandscapeType basicLandscape, LandscapeType waterLandscape, LandscapeType deepWaterLandscape, LandscapeType mountainsLandscape, LandscapeType rockLandscape)
{
    for (int x = 0; x < numberOfStructures; x++)
    {
        generateStructure(world, basicLandscape, waterLandscape, deepWaterLandscape, mountainsLandscape, rockLandscape);
    }

    return;
} // Generate structures