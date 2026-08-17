#include "updatelord.h"
#include "logging.h"
#include "dwarves.h"

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