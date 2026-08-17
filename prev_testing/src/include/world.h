#if !defined(WORLD_H)
#define WORLD_H

#include "types.h"
#include <time.h>

void initialize_world_landscapes(world *world);
void create_world_map(world *wrl);
void deselect_all_world_map(world *wrl, world_params_data_lord *world_params_data);
void create_dwarves(world *wrl, world_params_data_lord *world_params_data, prog_params_data_lord *prog_params_data, log_data_lord *log_data, draw_data_lord *draw_data);
void create_world_food(world *wrl, world_params_data_lord *world_params_data, draw_data_lord *draw_data);
void delete_world(world *wrl, world_params_data_lord *world_params_data, log_data_lord *log_data);
world *initialize_world(world_params_data_lord *world_params_data, prog_params_data_lord *prog_params_data, char *logs_barriers, coord map_size, log_data_lord *log_data, draw_data_lord *draw_data);

#endif