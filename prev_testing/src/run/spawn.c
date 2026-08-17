#include "spawn.h"
#include "types.h"

void spawn_dwarf(world *wrl, coord coords, dwarf dw, int x) {
    wrl->map[cell_id_in_map(coords.x, coords.y, wrl->map_size.x)].is_occupied = true;

    wrl->dwarves[x] = dw;
}