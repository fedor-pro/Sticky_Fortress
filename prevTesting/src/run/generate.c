#include <generate.h>
#include <time.h>
#include <stdlib.h>

bool structuresOverlayCheck(World *world, LandscapeStructure structure, Coord zoneStartCoord, int ZoneMaxLongX, int zoneMaxLongY, int l, int g) {
    if (world->map[(zoneStartCoord.x + l) + world->mapSize.x * (zoneStartCoord.y + g)].landType.gameId != world->worldLandscapes[0].gameId) { // Overlay check
        free(structure.incomingCellsCoords);
        return true;
    }

    return false;
}

void generateStructure(World *world)
{
    LandscapeStructure structure;

    float pond = rand() % 100 + rand() % 15;
    float pondIncrementStep = 1 + rand () % 3;
    int pondBorder = 200 + rand() % 100;

    if (rand() % 10 > 5)
    {
        structure.landscape = world->worldLandscapes[1];
        structure.secondLandscape = world->worldLandscapes[2];
    }
    else
    {
        structure.landscape = world->worldLandscapes[3];
        structure.secondLandscape = world->worldLandscapes[4];
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
            if (!(structure.landscape.gameId == LAND_MOUNTAINS)) // water
            {
                structure.incomingCellsCoords[j].x = zoneStartCoord.x + l;
                structure.incomingCellsCoords[j].y = zoneStartCoord.y + g;

                if (structuresOverlayCheck(world, structure, zoneStartCoord, zoneMaxLongX, zoneMaxLongY, l, g)) {
                    return;
                }

                if ((zoneMaxLongY/2) + (rand() % pondBorder)/2 >= pond) {
                    world->map[(zoneStartCoord.x + l) + world->mapSize.x * (zoneStartCoord.y + g)].landType = structure.secondLandscape;
                } else {
                    world->map[(zoneStartCoord.x + l) + world->mapSize.x * (zoneStartCoord.y + g)].landType = structure.landscape;
                } // so ugly generation
                
                j++;
            }
            else // mountains
            {
                if (((rand() % pondBorder) - 20) > pond)
                {
                    structure.incomingCellsCoords[j].x = zoneStartCoord.x + l;
                    structure.incomingCellsCoords[j].y = zoneStartCoord.y + g;

                    if (structuresOverlayCheck(world, structure, zoneStartCoord, zoneMaxLongX, zoneMaxLongY, l, g)) {
                        return;
                    }

                    if (((rand() % pondBorder + pond/2))/2 >= pond) {
                        world->map[(zoneStartCoord.x + l) + world->mapSize.x * (zoneStartCoord.y + g)].landType = structure.secondLandscape;
                    } else {
                        world->map[(zoneStartCoord.x + l) + world->mapSize.x * (zoneStartCoord.y + g)].landType = structure.landscape;
                    }

                    j++;
                }
            }

            pond += pondIncrementStep;
        }
    }

    free(structure.incomingCellsCoords);
}

void generateWorldStructures(World *world, worldParamsDataLord *worldParamsData)
{
    for (int x = 0; x < worldParamsData->structuresNumber; x++)
    {
        generateStructure(world);
    }

    return;
}