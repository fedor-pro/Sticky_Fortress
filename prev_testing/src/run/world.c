#include "world.h"
#include "generate.h"
#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include "spawn.h"
#include "logging.h"

void initialize_world_landscapes(world *wrl) 
{
    landscape_type basic_landscape = {LAND_BASIC, "Basic landscape", true, (Color) {3, 130, 0, 225}};
    landscape_type water_landscape = {LAND_WATER, "Water surface", false, (Color){0, 0, 120, 255}};
    landscape_type deep_water_landscape = {LAND_DEEP_WATER, "Deep water", false, (Color){0, 0, 50, 255}};
    landscape_type mountains_landscape = {LAND_MOUNTAINS, "Mountains and hills", true, (Color){118, 114, 113, 255}};
    landscape_type rock_landscape = {LAND_ROCK, "Hard rock landscape", false, (Color){68, 64, 63, 255}};

    wrl->world_landscapes[0] = basic_landscape;
    wrl->world_landscapes[1] = water_landscape;
    wrl->world_landscapes[2] = deep_water_landscape;
    wrl->world_landscapes[3] = mountains_landscape;
    wrl->world_landscapes[4] = rock_landscape;

    wrl->world_landscapes[0] = (landscape_type) {LAND_BASIC, "Basic landscape", true, (Color) {3, 130, 0, 225}};
}

void initialize_world_file(world *wrl, prog_params_data_lord *prog_params_data, world_params_data_lord *world_params_data) {
    
    char *world_file_path = malloc(prog_params_data->text_buffer_size);
    sprintf(world_file_path, "./worlds/%s.txt", world_params_data->default_name);

    wrl->world_file = fopen(world_file_path, "w");

    fflush(wrl->world_file);
}

void create_world_map(world *wrl)
{
    for (int x = 0; x < wrl->map_size.x; x++)
    {
        for (int y = 0; y < wrl->map_size.y; y++)
        {
            landscape_cell ld;

            ld.l_coord = (coord){x, y};

            ld.is_selected = false;
            ld.is_occupied = false;

            ld.land_type = wrl->world_landscapes[0];

            wrl->map[cell_id_in_map(x, y, wrl->map_size.x)] = ld;
        }
    }
}

void deselect_all_world_map(world *wrl, world_params_data_lord *world_params_data) {
    for (int x = 0; x < wrl->map_size.x; x++)
    {
        for (int y = 0; y < wrl->map_size.y; y++)
        {
            wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].is_selected = false;
        }
    }

    for (int u = 0; u < 5; u++) // Reset selected landscape cells stats
    {
        world_params_data->cells_selected[u] = 0;
    }
}

void create_dwarves(world *wrl, world_params_data_lord *world_params_data, prog_params_data_lord *prog_params_data, log_data_lord *log_data, draw_data_lord *draw_data)
{
    for (int x = 0; x < world_params_data->start_dwarves_number; x++)
    {
        int dw_x = rand() % wrl->map_size.x;
        int dw_y = rand() % wrl->map_size.y;

        while (!wrl->map[cell_id_in_map(dw_x, dw_y, wrl->map_size.x)].land_type.can_be_occupied)
        {
            dw_x = rand() % wrl->map_size.x;
            dw_y = rand() % wrl->map_size.y;
        }

        char *dwarf_game_id = malloc(prog_params_data->text_buffer_size);
        sprintf(dwarf_game_id, "%d", x);

        dwarf dw = {dwarf_game_id, "Dwarf", true, true, draw_data->default_human_char, dw_x, dw_y, UNTARGET_MOVING, -1, 0, 0, 0, 50 + rand () % 1000 + rand () % 50, 0, RED};
        //                              dwarvenness  is                                                      target target hunger die level sleepiness
        //                                           alive                                                  food id   cell coords    hunger   

        if (x > 0 && x < world_params_data->start_dwarves_number - 1) {

            if (x == 1) {
                raw_log_to_file(log_data, "...\n");
            }
        } else {
            log_to_file(log_data, "Created ");
            raw_log_to_file(log_data, dw.game_name);
            raw_log_to_file(log_data, " with id: |");
            raw_log_to_file(log_data, dw.game_id);
            raw_log_to_file(log_data,"|\n");
        }

        time(&log_data->raw_time);
        log_data->tm = localtime(&log_data->raw_time); // Updating time

        spawn_dwarf(wrl, (coord) {dw_x, dw_y}, dw, x);
    }
}

void create_world_food(world *wrl, world_params_data_lord *world_params_data, draw_data_lord *draw_data)
{
    for (int x = 0; x < world_params_data->start_food_on_map; x++)
    {
        int res_x = rand() % wrl->map_size.x;
        int res_y = rand() % wrl->map_size.y;

        while (!wrl->map[cell_id_in_map(res_x, res_y, wrl->map_size.x)].land_type.can_be_occupied)
        {
            res_x = rand() % wrl->map_size.x;
            res_y = rand() % wrl->map_size.y;
        }

    item f = {res_x, res_y, FOOD, draw_data->default_food_char, 50 + rand() % 30, true};

        wrl->items[x] = f;
    }
}

void delete_world(world *wrl, world_params_data_lord *world_params_data, log_data_lord *log_data)
{
    fprintf(wrl->world_file, "==DATA==\n");
    fprintf(wrl->world_file, "[WORLD_NAME]:%s\n", wrl->world_name);
    fflush(wrl->world_file);

    fprintf(wrl->world_file, "[DWARVES_ALIVE]:%d\n", world_params_data->dwarves_alive);
    fprintf(wrl->world_file, "[FOOD_REMAINING]:%d\n", world_params_data->food_exists);
    fflush(wrl->world_file);

    fprintf(wrl->world_file, "\n==DWARVES_LIST==\n");

    for (int x = 0; x < world_params_data->start_dwarves_number; x++)
    {
        fprintf(wrl->world_file, "{DWARF}[ID]:%s|[GAME_NAME]:%s|[DWARVENNESS]:%d|[IS_ALIVE]:%d|[DRAWING_CHAR]:%s|[COORDS]:%d,%d|[HUNGER]:%f|[DIE_LEVEL_HUNGER]:%f|[SLEEPINESS]:%f\n", 
            wrl->dwarves[x].game_id, wrl->dwarves[x].game_name, wrl->dwarves[x].dwarvenness, wrl->dwarves[x].is_alive, wrl->dwarves[x].char_value,
            wrl->dwarves[x].coords.x, wrl->dwarves[x].coords.y, wrl->dwarves[x].hunger, wrl->dwarves[x].die_level_hunger, wrl->dwarves[x].sleepiness);

        fflush(wrl->world_file);
    }

    fprintf(wrl->world_file, "\n==WORLD_MAP==\n");

    for (int xx = 0; xx < wrl->map_size.x; xx++)
    {
        for (int yy = 0; yy < wrl->map_size.y; yy++)
        { 
            fprintf(wrl->world_file, "{CELL}[COORDS]:%d,%d|[IS_SELECTED]:%d|[IS_OCCUPIED]:%d|[LANDSCAPE]:%d", 
                wrl->map[cell_id_in_map(xx, yy, wrl->map_size.x)].l_coord.x, wrl->map[cell_id_in_map(xx, yy, wrl->map_size.x)].l_coord.y, 
                wrl->map[cell_id_in_map(xx, yy, wrl->map_size.x)].is_selected, wrl->map[cell_id_in_map(xx, yy, wrl->map_size.x)].is_occupied, wrl->map[cell_id_in_map(xx, yy, wrl->map_size.x)].land_type);
            
            fflush(wrl->world_file);
        }

        fprintf(wrl->world_file, "\n");
    }

    for (int i = 0; i < world_params_data->start_dwarves_number; i++)
    {
        if (wrl->dwarves[i].game_id != NULL)
        {
            free(wrl->dwarves[i].game_id);
        }
    }

    free(wrl->map);
    free(wrl->world_landscapes);
    free(wrl->dwarves);
    free(wrl->items);

    fclose(wrl->world_file);
    free(wrl);
}

world *initialize_world(world_params_data_lord *world_params_data, prog_params_data_lord *prog_params_data, char *logs_barriers, coord map_size, log_data_lord *log_data, draw_data_lord *draw_data)
{
    world *wrl = malloc(sizeof(world));
    wrl->world_name = world_params_data->default_name;
    wrl->map_size = map_size;
    wrl->map = malloc(sizeof(landscape_cell) * (wrl->map_size.x * wrl->map_size.y)); // Creating map

    wrl->world_landscapes = malloc(sizeof(landscape_type)*10);
    initialize_world_landscapes(wrl);

    initialize_world_file(wrl, prog_params_data, world_params_data);

    create_world_map(wrl);
    log_to_file(log_data, "INITIALIZED WORLD MAP\n");
    raw_log_to_file(log_data, logs_barriers);

    generate_world_structures(wrl, world_params_data);

    log_to_file(log_data, "Путукфеув ыекгсегкуы\n");
    raw_log_to_file(log_data, logs_barriers);

    wrl->dwarves = malloc(sizeof(dwarf) * (world_params_data->start_dwarves_number * 1.5)); // Creating dwarves

    create_dwarves(wrl, world_params_data, prog_params_data, log_data, draw_data);

    raw_log_to_file(log_data, logs_barriers);

    wrl->items = malloc(sizeof(landscape_cell) * (world_params_data->start_food_on_map + 5)); // Creating items

    create_world_food(wrl, world_params_data, draw_data);

    deselect_all_world_map(wrl, world_params_data);

    return wrl;
}