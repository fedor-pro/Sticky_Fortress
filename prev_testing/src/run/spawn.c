#include "spawn.h"
#include "types.h"

void spawn_dwarf(world *wrl, coord coords, dwarf dw, int x) {
    wrl->map[coords.x+ wrl->map_size.x * coords.y].is_occupied = true;

    wrl->dwarves[x] = dw;
}