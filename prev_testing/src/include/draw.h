#if !defined(DRAW_H)
#define DRAW_H

#include "types.h"

void draw_gui_pannel(gui_pannel guip);
void draw_gui_text(gui_text guitx);
void draw_dwarf(dwarf dw, prog_params_data_lord *prog_params_data);
void draw_item(item i, prog_params_data_lord *prog_params_data);

#endif