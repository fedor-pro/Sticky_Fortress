// STANDALONE MODULE

#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma once
#include "types.h"

void spawnEntity(World *world, Coord coords, Entity e, int x) {
    world->map[coords.x+ world->mapSize.x * coords.y].isOccupied = true;

    world->entities[x] = e;
}