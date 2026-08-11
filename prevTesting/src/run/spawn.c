#include "spawn.h"

void spawnDwarf(World *world, Coord coords, Dwarf dw, int x) {
    world->map[coords.x+ world->mapSize.x * coords.y].isOccupied = true;

    world->dwarves[x] = dw;
}