#if !defined(GENERATE_H)
#define GENERATE_H

#include "types.h"

bool structures_overlay_check(world *wrl, landscape_structure structure, coord zone_start_coord, int zone_max_long_x, int zone_max_long_y, int l, int g);
void generate_structure(world *wrl);
void generate_world_structures(world *wrl, world_params_data_lord *world_params_data);

#endif