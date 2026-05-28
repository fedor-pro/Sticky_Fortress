#include "spawn.h"

void spawnEntity(World *world, Coord coords, Entity e, int x) {
    world->map[coords.x+ world->mapSize.x * coords.y].isOccupied = true;

    world->entities[x] = e;
}