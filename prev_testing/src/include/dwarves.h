#if !defined(DWARVES_H)
#define DWARVES_H

#include "types.h"
#include <time.h>

void move_dwarf(direction direct, dwarf *dw, world *wrl);
void restore_hunger_dwarf(world  *wrl, dwarf *dw, world_params_data_lord *world_params_data, log_data_lord *log_data);
bool find_nearest_food(world *wrl, dwarf *dw, world_params_data_lord *world_params_data);
void update_dwarf(world *wrl, coord map_size, dwarf *dw, int timer, world_params_data_lord *world_params_data, log_data_lord *log_data) ;

#endif 