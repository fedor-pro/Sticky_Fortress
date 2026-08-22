#if !defined(UILORD_H)
#define UILORD_H

#include "types.h"
#include <stdlib.h>

ui_lord *initialize_ui_lord (prog_params_data_lord *prog_params_data, int default_font_size);
void delete_ui_lord (ui_lord *uil);
void update_ui_lord (ui_lord *uil, world_params_data_lord *world_params_data, prog_params_data_lord *prog_params_data);
void draw_ui_lord (ui_lord *UIL); 

#endif