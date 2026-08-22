#include "updatelord.h"
#include "logging.h"
#include "dwarves.h"
#include "world.h"

void update_game_running_params(prog_params_data_lord *prog_params_data, log_data_lord *log_data) { // Timer, fps, pause, time...
    time(&log_data->raw_time);
    log_data->tm = localtime(&log_data->raw_time);

    prog_params_data->current_fps = GetFPS();

    if (IsKeyPressed(KEY_SPACE)) // Pause
    {
        prog_params_data->is_paused = !prog_params_data->is_paused;
    }

    if (!prog_params_data->is_paused)
    {
        prog_params_data->timer++; // Update timer
    }
    
    if (prog_params_data->square_selecting_freeze > 0) { // Update freeze time for selecting 
        prog_params_data->square_selecting_freeze --;
    }
}

void update_game_objects(world *wrl, prog_params_data_lord *prog_params_data, world_params_data_lord *world_params_data, log_data_lord *log_data) { // Dwarves, items...
    for (int x = 0; x < world_params_data->start_dwarves_number; x++) // Update dwarves
    {
        if (wrl->dwarves[x].is_alive == true && !prog_params_data->is_paused)
        {
            update_dwarf(wrl, wrl->map_size, &wrl->dwarves[x], prog_params_data->timer, world_params_data, log_data);
        }
    } 
    
    if (!prog_params_data->is_paused && prog_params_data->timer >= prog_params_data->timer_reset) // Update items
    {
        prog_params_data->timer = 0;
        
        for (int u = 0; u < world_params_data->start_food_on_map; u ++) {
            if (wrl->items[u].number <= 0) {
                world_params_data->food_exists --;
                wrl->items[u].is_exist = false; // <-- This need to replace with removal from array
            }

            if ((rand() % 500) == 1) {
                wrl->items[u].number ++;

                char *si = malloc(sizeof(char)*12);
                sprintf(si, "%d", u);

                log_to_file(log_data, "Food with id |");
                raw_log_to_file(log_data, si);
                raw_log_to_file(log_data, "| restored\n");
            }
        }

        sprintf(prog_params_data->string_current_fps, "%d", prog_params_data->current_fps);
        log_to_file(log_data, "Current FPS: ");
        raw_log_to_file(log_data, prog_params_data->string_current_fps);
        raw_log_to_file(log_data, "\n");
    }
}

void update_game_stats(world *wrl, world_params_data_lord *world_params_data) {
    for (int u = 0; u < 5; u++) // Reset selected landscape cells stats
    {
        world_params_data->cells_selected[u] = 0;
    }

    for (int x = 0; x < wrl->map_size.x; x++) // Drawing map
    {
        for (int y = 0; y < wrl->map_size.y; y++)
        {
            if (wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].is_selected == true) // If cell is selected
            {
                if (wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].land_type.game_id == LAND_BASIC)
                {
                    world_params_data->cells_selected[0]++;
                }
                else if (wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].land_type.game_id == LAND_WATER)
                {
                    world_params_data->cells_selected[1]++;
                }
                else if (wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].land_type.game_id == LAND_MOUNTAINS)
                {
                    world_params_data->cells_selected[2]++;
                }
                else if (wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].land_type.game_id == LAND_ROCK)
                {
                    world_params_data->cells_selected[3]++;
                }
                else if (wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].land_type.game_id == LAND_DEEP_WATER)
                {
                    world_params_data->cells_selected[4]++;
                }
            }
        }
    }
}

void update_selection(world *wrl, world_params_data_lord *world_params_data, prog_params_data_lord *prog_params_data) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) // Selecting cells | In update lord
    {
        if (prog_params_data->square_selecting_freeze == 0) 
        {
            prog_params_data->if_square_selecting_active = !prog_params_data->if_square_selecting_active;

            if (prog_params_data->if_square_selecting_active) 
            {
                prog_params_data->square_selecting_start_cell_coords = prog_params_data->mouse_position;
                wrl->map[cell_id_in_map((prog_params_data->square_selecting_start_cell_coords.x/prog_params_data->rect_size.x), (prog_params_data->square_selecting_start_cell_coords.y/prog_params_data->rect_size.y), wrl->map_size.x)].is_selected = true;
            } 
            else 
            {
                // Select square from (c1.x; c1.y) to (c2.x; c2.y)
                // Go from selecting start to selecting end

                prog_params_data->square_selecting_end_cell_coords.x = (prog_params_data->square_selecting_start_cell_coords.x > prog_params_data->mouse_position.x) ? prog_params_data->square_selecting_start_cell_coords.x : prog_params_data->mouse_position.x;
                prog_params_data->square_selecting_end_cell_coords.y = (prog_params_data->square_selecting_start_cell_coords.y > prog_params_data->mouse_position.y) ? prog_params_data->square_selecting_start_cell_coords.y : prog_params_data->mouse_position.y;

                prog_params_data->square_selecting_start_cell_coords.x = (prog_params_data->square_selecting_start_cell_coords.x < prog_params_data->mouse_position.x) ? prog_params_data->square_selecting_start_cell_coords.x : prog_params_data->mouse_position.x;
                prog_params_data->square_selecting_start_cell_coords.y = (prog_params_data->square_selecting_start_cell_coords.y < prog_params_data->mouse_position.y) ? prog_params_data->square_selecting_start_cell_coords.y : prog_params_data->mouse_position.y;

                for (int ab = (prog_params_data->square_selecting_start_cell_coords.x / prog_params_data->rect_size.x); ab < ((prog_params_data->square_selecting_end_cell_coords.x + prog_params_data->rect_size.x - 1) / prog_params_data->rect_size.x); ab ++) 
                {
                    for (int ord = (prog_params_data->square_selecting_start_cell_coords.y / prog_params_data->rect_size.y); ord < ((prog_params_data->square_selecting_end_cell_coords.y + prog_params_data->rect_size.y - 1) / prog_params_data->rect_size.y); ord ++) 
                    {
                        if (wrl->map[cell_id_in_map(ab, ord, wrl->map_size.x)].is_selected == false) 
                        {
                            wrl->map[cell_id_in_map(ab, ord, wrl->map_size.x)].is_selected = true; 
                        }
                    }
                }
            }

            prog_params_data->square_selecting_freeze = 30;
        }
    } 
    else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) // STUB
    {
        if ((prog_params_data->square_selecting_start_cell_coords.x/prog_params_data->rect_size.x) < prog_params_data->window_size.x && (prog_params_data->square_selecting_start_cell_coords.x/prog_params_data->rect_size.x) > 0
            &&(prog_params_data->square_selecting_start_cell_coords.y/prog_params_data->rect_size.y) < prog_params_data->window_size.y && (prog_params_data->square_selecting_start_cell_coords.y/prog_params_data->rect_size.y) > 0
            && wrl->map[cell_id_in_map((prog_params_data->mouse_position.x/prog_params_data->rect_size.x), (prog_params_data->mouse_position.y/prog_params_data->rect_size.y), wrl->map_size.x)].is_selected)
        {
            wrl->map[cell_id_in_map((prog_params_data->mouse_position.x/prog_params_data->rect_size.x), (prog_params_data->mouse_position.y/prog_params_data->rect_size.y), wrl->map_size.x)].is_selected = false;
        }
    } 
    else if (IsKeyDown(KEY_ESCAPE))
    {
        deselect_all_world_map(wrl, world_params_data);
    }

}