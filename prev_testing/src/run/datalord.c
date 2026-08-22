#include "types.h"
#include <stdlib.h>
#include <time.h>

prog_params_data_lord *define_prog_params_data_lord () {
    prog_params_data_lord *prog_params_data = malloc(sizeof(prog_params_data_lord));

    prog_params_data->window_size = (coord) {1900, 1000};
    prog_params_data->rect_size = (coord) {15, 15};
    prog_params_data->text_buffer_size = 500;

    prog_params_data->timer = 0;
    prog_params_data->current_fps = 0;
    prog_params_data->string_current_fps = malloc(prog_params_data->text_buffer_size);
    prog_params_data->is_paused = false;

    prog_params_data->timer_reset = 60;

    prog_params_data->if_square_selecting_active = false;
    prog_params_data->square_selecting_start_cell_coords;
    prog_params_data->square_selecting_end_cell_coords;
    prog_params_data->square_selecting_freeze = 0;

    prog_params_data->mouse_position = (coord) {0, 0};

    return prog_params_data;
}

world_params_data_lord *define_world_params_data_lord () {
    world_params_data_lord *world_params_data = malloc(sizeof(world_params_data_lord));

    world_params_data->default_name = "test_world";

    world_params_data->start_dwarves_number = 100;
    world_params_data->start_food_on_map = 15;
    world_params_data->structures_number = 10;

    world_params_data->dwarves_alive = world_params_data->start_dwarves_number;
    world_params_data->dwarves_selected = 0;
    world_params_data->food_exists = world_params_data->start_food_on_map;


    world_params_data->cells_selected = malloc(sizeof(int)*5);

    return world_params_data;
}

draw_data_lord *define_draw_data_lord () {
    draw_data_lord *draw_data = malloc(sizeof(draw_data_lord));

    draw_data->default_font_size = 23;
    draw_data->default_food_char = "*";
    draw_data->default_human_char = "&";

    return draw_data;
}

log_data_lord *define_log_data_lord () {
    log_data_lord *log_data = malloc(sizeof(log_data_lord));

    log_data->raw_time = time(NULL);
    log_data->tm = localtime(&(log_data->raw_time));
    time(&log_data->raw_time);

    // log_data->source_log_file will be initialize in logging.c    

    return log_data;
}

void undefine_all_data_lords (prog_params_data_lord *prog_params_data, world_params_data_lord *world_params_data, draw_data_lord *draw_data, log_data_lord *log_data) {
    free(prog_params_data);
    free(world_params_data->cells_selected);
    free(world_params_data);
    free(draw_data);
    free(log_data);
}