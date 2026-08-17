#include "uilord.h"
#include "draw.h"
#include <raylib.h>

ui_lord *initialize_ui_lord (prog_params_data_lord *prog_params_data, int default_font_size) {
    Color ui_pannels_back = (Color) {0, 0, 0, 185};

    gui_pannel mouse_info = {.start_coords.x = 0, .start_coords.y = 0, .canv_size_coords.x = 280, .canv_size_coords.y = 150, .background_color = ui_pannels_back};

    gui_text mouse_coords_text= {.text = (char *)malloc(prog_params_data->text_buffer_size), .start_coords.x = mouse_info.start_coords.x + 15, .start_coords.y = mouse_info.start_coords.y + 25, .font_size = default_font_size, .font_color = RED};

    gui_pannel selected_cells_info = {.start_coords.y = 0, .canv_size_coords.x = 300, .canv_size_coords.y = 210, .background_color =  ui_pannels_back};
    selected_cells_info.start_coords.x = prog_params_data->window_size.x - selected_cells_info.canv_size_coords.x;

    gui_text selected_cells_text = {.text = (char *)malloc(prog_params_data->text_buffer_size), .start_coords.x = selected_cells_info.start_coords.x + 15, .start_coords.y = selected_cells_info.start_coords.y + 25, .font_size = default_font_size, .font_color = GOLD};
    gui_text world_save_text = {.text = (char *)malloc(prog_params_data->text_buffer_size), .start_coords.x = selected_cells_info.start_coords.x + 15, .start_coords.y = selected_cells_text.start_coords.y  + 140, .font_size = default_font_size-5, .font_color = RED};

    gui_pannel dwarves_info = {.start_coords.x = 0, .canv_size_coords.x = 300, .canv_size_coords.y = 200, .background_color =  ui_pannels_back};
    dwarves_info.start_coords.y = prog_params_data->window_size.y - dwarves_info.canv_size_coords.y;

    gui_text dwarves_number_text = {.text = (char *)malloc(prog_params_data->text_buffer_size), .start_coords.x = dwarves_info.start_coords.x + 5, .start_coords.y = dwarves_info.start_coords.y + 25, .font_size = default_font_size, .font_color = GREEN};
    gui_text dwarves_selected_text = {.text = (char *)malloc(prog_params_data->text_buffer_size), .start_coords.x = dwarves_info.start_coords.x + 5, .start_coords.y = dwarves_info.start_coords.y + 60, .font_size = default_font_size, .font_color = GREEN};

    gui_pannel frame_info = {.start_coords.x = prog_params_data->window_size.x - 200, .start_coords.y = prog_params_data->window_size.y- 200, .canv_size_coords.x = 200, .canv_size_coords.y = 200, .background_color =  ui_pannels_back};

    gui_text frame_info_text = {.text = (char *)malloc(prog_params_data->text_buffer_size), .start_coords.x = frame_info.start_coords.x + 5, .start_coords.y = frame_info.start_coords.y + 25, .font_size = default_font_size-5, .font_color = RED};

    ui_lord *uil = malloc(sizeof(ui_lord));
    uil->all_gui_pannels = malloc(sizeof(gui_pannel)*10);
    uil->all_gui_text = malloc(sizeof(gui_text)*10);

    uil->all_gui_pannels[0] = mouse_info;
    uil->all_gui_pannels[1] = selected_cells_info;
    uil->all_gui_pannels[2] = dwarves_info;
    uil->all_gui_pannels[3] = frame_info;

    uil->all_gui_text[0] = mouse_coords_text;
    uil->all_gui_text[1] = selected_cells_text;
    uil->all_gui_text[2] = dwarves_number_text;
    uil->all_gui_text[3] = frame_info_text;
    uil->all_gui_text[4] = dwarves_selected_text;
    uil->all_gui_text[5] = world_save_text;

    return uil;
}

void delete_ui_lord (ui_lord *uil) {
    free(uil->all_gui_pannels);
    free(uil->all_gui_text);
    free(uil);
}

void update_ui_lord (ui_lord *uil, coord mouse_position, world_params_data_lord *world_params_data, prog_params_data_lord *prog_params_data) {
    sprintf(uil->all_gui_text[0].text, "X: %d Y: %d", mouse_position.x, mouse_position.y);

    sprintf(uil->all_gui_text[1].text, "Selected: \nbasic landscape: %d; \nwater: %d; \nmountains: %d; \nrocks: %d; deep water: %d", 
        world_params_data->cells_selected[0], world_params_data->cells_selected[1], world_params_data->cells_selected[2], world_params_data->cells_selected[3], world_params_data->cells_selected[4]);
    sprintf(uil->all_gui_text[5].text, "You cand find world map \nin prevTesting/worlds/");

    sprintf(uil->all_gui_text[2].text, "Dwarves alive: %d / %d", world_params_data->dwarves_alive, world_params_data->start_dwarves_number);
    sprintf(uil->all_gui_text[4].text, "      selected: %d", world_params_data->dwarves_selected);

    if (prog_params_data->is_paused) {
        sprintf(uil->all_gui_text[3].text, "Frame (from 1 to 60) :\n %d\n Is paused :\n true\n\n You can find FPS\n in log files", prog_params_data->timer);
    } else {
        sprintf(uil->all_gui_text[3].text, "Frame (from 1 to 60) :\n %d\n Is paused :\n false\n\n You can find FPS\n in log files", prog_params_data->timer);
    }
}

void draw_ui_lord (ui_lord *uil) {
    for (int x = 0; x < 4; x ++) {
        draw_gui_pannel(uil->all_gui_pannels[x]);
    }

    for (int y = 0; y < 6; y ++) {
        draw_gui_text(uil->all_gui_text[y]);
    }

    DrawText("Lmb to select square, \nrmb to deselect area.", uil->all_gui_text[0].start_coords.x, uil->all_gui_text[0].start_coords.y + 30, 23, GREEN);
}