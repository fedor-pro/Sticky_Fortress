#include "draw.h"
#include <raylib.h>

void draw_gui_pannel(gui_pannel guip) // Draw pannel
{
    DrawRectangle(guip.start_coords.x, guip.start_coords.y, guip.canv_size_coords.x, guip.canv_size_coords.y, guip.background_color);

    DrawRectangle(guip.start_coords.x + (guip.canv_size_coords.x/50), (guip.start_coords.y + guip.canv_size_coords.y) - (guip.canv_size_coords.y/30), guip.canv_size_coords.x - ((guip.canv_size_coords.x/50)*2), 1, (Color) {255, 255, 255, 120});
    DrawRectangle(guip.start_coords.x + (guip.canv_size_coords.x/50), (guip.start_coords.y + guip.canv_size_coords.y) - (guip.canv_size_coords.y/30) + 2, guip.canv_size_coords.x - ((guip.canv_size_coords.x/50)*2), 1, (Color) {255, 255, 255, 120});

    DrawRectangle(guip.start_coords.x + (guip.canv_size_coords.x/50), guip.start_coords.y + (guip.canv_size_coords.y/30), guip.canv_size_coords.x - ((guip.canv_size_coords.x/50)*2), 1, (Color) {255, 255, 255, 120});
    DrawRectangle(guip.start_coords.x + (guip.canv_size_coords.x/50), guip.start_coords.y + (guip.canv_size_coords.y/30) + 2, guip.canv_size_coords.x - ((guip.canv_size_coords.x/50)*2), 1, (Color) {255, 255, 255, 120});
}

void draw_gui_text(gui_text guitx) // Draw text
{
    DrawText(guitx.text, guitx.start_coords.x, guitx.start_coords.y, guitx.font_size, guitx.font_color);
}

void draw_dwarf(dwarf dw, prog_params_data_lord *prog_params_data) // Draw dwarf(ASCII)
{
    DrawText(dw.char_value, dw.coords.x * prog_params_data->rect_size.x + 1, dw.coords.y * prog_params_data->rect_size.y - 1, prog_params_data->rect_size.y * 1.3, dw.drawing_color);
}

void draw_item(item i, prog_params_data_lord *prog_params_data) // Draw item(ASCII)
{
    DrawText(i.char_value, i.coords.x * prog_params_data->rect_size.x + 1, i.coords.y * prog_params_data->rect_size.y - 1, prog_params_data->rect_size.y * 1.3, YELLOW);
}