#if !defined(UPDATELORD_H)
#define UPDATELORD_H

#include "types.h"

void update_game_running_params(prog_params_data_lord *prog_params_data, log_data_lord* log_data);

void update_game_objects(world *wrl, prog_params_data_lord *prog_params_data, world_params_data_lord *world_params_data, log_data_lord *log_data);

void update_game_stats(world *wrl, world_params_data_lord *world_params_data);

void update_selection(world *wrl, world_params_data_lord *world_params_data, prog_params_data_lord *prog_params_data);

#endif