#if !defined(DATALORD_H)
#define DATALORD_H

#include "types.h"
#include "stdlib.h"

prog_params_data_lord *define_prog_params_data_lord ();
world_params_data_lord *define_world_params_data_lord ();
draw_data_lord *define_draw_data_lord ();
log_data_lord *define_log_data_lord ();
void undefine_all_data_lords(prog_params_data_lord *prog_params_data, world_params_data_lord *world_params_data, draw_data_lord *draw_data, log_data_lord *log_data);

#endif