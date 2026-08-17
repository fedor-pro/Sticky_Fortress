#include <stdio.h>
#include <raylib.h>
#include <stdlib.h> // For rand() func
#include <time.h>   // For timestamp and rand() initialize

#include "dwarves.h"
#include "logging.h"
#include "uilord.h"
#include "world.h"
#include "datalord.h"
#include "draw.h"
#include "updatelord.h"

#define VERSION "0.0.2"

#define TARGET_FPS 60

#define LOGS_BARRIERS "---------------------------------------------------------------\n"

int main()
{
    SetTraceLogLevel(LOG_NONE); // For delete all raylib's sys logs

    // Creating data lords
    prog_params_data_lord *prog_params_data = define_prog_params_data_lord();
    world_params_data_lord *world_params_data = define_world_params_data_lord();
    draw_data_lord *draw_data = define_draw_data_lord();
    log_data_lord*log_data = define_log_data_lord();

    prog_params_data->timer = 0;
    prog_params_data->current_fps = 0;
    prog_params_data->is_paused = false;

    bool if_square_selecting_active = false;
    coord square_selecting_start_cell_coords;
    coord square_selecting_end_cell_coords;
    int square_selecting_freeze = 0;

    srand(time(NULL));

    // Initializing log file
    char *source_log_file_path = malloc(prog_params_data->text_buffer_size);

    initialize_log_file(source_log_file_path, log_data);

    if (log_data->source_log_file == NULL)
    {
        printf("Not found %s\n", source_log_file_path);
        free(source_log_file_path);
        return 1;
    }

    raw_log_to_file(log_data, LOGS_BARRIERS);
    log_to_file(log_data, "PROGRAM STARTED\n");
    raw_log_to_file(log_data, LOGS_BARRIERS);

    coord ms = {prog_params_data->window_size.x / prog_params_data->rect_size.x, prog_params_data->window_size.y / prog_params_data->rect_size.y};

    // Log start info
    char *init_log_info = malloc(prog_params_data->text_buffer_size);
    sprintf(init_log_info, "%s %d,%d\n\n", "Defined window size", prog_params_data->window_size.x, prog_params_data->window_size.y);
    log_to_file(log_data, init_log_info);

    sprintf(init_log_info, "%s %d,%d\n\n", "Defined cell size", prog_params_data->rect_size.x, prog_params_data->rect_size.y);
    log_to_file(log_data, init_log_info);
    
    sprintf(init_log_info, "Defined map size %d, %d\n", ms.x, ms.y);
    log_to_file(log_data, init_log_info);

    raw_log_to_file(log_data, LOGS_BARRIERS);

    // Creating world
    world *wrl = initialize_world(world_params_data, prog_params_data, LOGS_BARRIERS, ms, log_data, draw_data);

    // Initializing window
    char *window_name = malloc(prog_params_data->text_buffer_size);
    sprintf(window_name, "Sticky Fortress %s", VERSION);

    InitWindow(prog_params_data->window_size.x, prog_params_data->window_size.y, window_name);
    SetTargetFPS(TARGET_FPS);

    SetExitKey(KEY_NULL); // If active, window willn't close on ESC button

    Image window_icon = LoadImage("./images/windowIcon.png"); // Loading icon
    if (window_icon.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) // Formatting icon
    {
        ImageFormat(&window_icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    }
    ImageColorReplace(&window_icon, WHITE, BLANK);
    SetWindowIcon(window_icon);

    log_to_file(log_data, "INITIALIZED WINDOW\n");

    // Initialize main UI
    ui_lord *ui_central = initialize_ui_lord(prog_params_data, draw_data->default_font_size);

    raw_log_to_file(log_data,  LOGS_BARRIERS);
    log_to_file(log_data, "STARTED APP\n");
    raw_log_to_file(log_data,  LOGS_BARRIERS);

    while (!WindowShouldClose())
    {
        update_game_running_params(prog_params_data, log_data);

        BeginDrawing();

        world_params_data->dwarves_alive = 0;
        world_params_data->dwarves_selected = 0;

        if (square_selecting_freeze > 0) {
            square_selecting_freeze --;
        }

        ClearBackground(BLACK); // Clear background

        update_game_objects(wrl, prog_params_data, world_params_data, log_data);

        for (int x = 0; x < wrl->map_size.x; x++) // Drawing map
        {
            for (int y = 0; y < wrl->map_size.y; y++)
            {
                if (wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].is_selected == 1) // If cell is selected
                {
                    DrawRectangle(x * prog_params_data->rect_size.x, y * prog_params_data->rect_size.y, prog_params_data->rect_size.x + 1, prog_params_data->rect_size.y + 1, GOLD); 
                }

                DrawRectangle(x * prog_params_data->rect_size.x + 1, y * prog_params_data->rect_size.y + 1, prog_params_data->rect_size.x - 1, prog_params_data->rect_size.y - 1, wrl->map[cell_id_in_map(x, y, wrl->map_size.x)].land_type.draw_color);
            }
        }

        for (int x = 0; x < world_params_data->start_food_on_map; x++) // Draw items
        {
            if (wrl->items[x].number > 0)
            {
                draw_item(wrl->items[x], prog_params_data);
            }
        }

        for (int x = 0; x < world_params_data->start_dwarves_number; x++) // Draw dwarves
        {
            draw_dwarf(wrl->dwarves[x], prog_params_data);
        }

        Vector2 mp = GetMousePosition(); // Updating info about mouse position
        coord mouse_position = {(int) mp.x, (int) mp.y};

        update_ui_lord(ui_central, mouse_position, world_params_data, prog_params_data); // Update main UI 
        draw_ui_lord(ui_central); // Draw main UI

        // for (int u = 0; u < 5; u++) // Reset selected landscape cells stats
        // {
        //     selected_cells[u] = 0;
        // }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) // Selecting cells
        {
            if (square_selecting_freeze == 0) {
                if_square_selecting_active = !if_square_selecting_active;

                if (if_square_selecting_active) 
                {
                    deselect_all_world_map(wrl, world_params_data);

                    square_selecting_start_cell_coords = mouse_position;
                    wrl->map[cell_id_in_map((square_selecting_start_cell_coords.x/prog_params_data->rect_size.x), (square_selecting_start_cell_coords.y/prog_params_data->rect_size.y), wrl->map_size.x)].is_selected = true;
                } 
                else 
                {
                    for (int u = 0; u < 5; u++) // Reset selected landscape cells stats
                    {
                        world_params_data->cells_selected[u] = 0;
                    }

                    // Select square from (c1.x; c1.y) to (c2.x; c2.y)
                    // Go from selecting start to selecting end

                    square_selecting_end_cell_coords.x = (square_selecting_start_cell_coords.x > mouse_position.x) ? square_selecting_start_cell_coords.x : mouse_position.x;
                    square_selecting_end_cell_coords.y = (square_selecting_end_cell_coords.y > mouse_position.y) ? square_selecting_end_cell_coords.y : mouse_position.y;

                    square_selecting_start_cell_coords.x = (square_selecting_start_cell_coords.x < mouse_position.x) ? square_selecting_start_cell_coords.x : mouse_position.x;
                    square_selecting_start_cell_coords.y = (square_selecting_start_cell_coords.y < mouse_position.y) ? square_selecting_start_cell_coords.y : mouse_position.y;

                    for (int ab = square_selecting_start_cell_coords.x; ab < square_selecting_end_cell_coords.x; ab ++) 
                    {
                        for (int ord = square_selecting_start_cell_coords.y; ord < square_selecting_end_cell_coords.y; ord ++) 
                        {
                            wrl->map[cell_id_in_map((ab/prog_params_data->rect_size.x), (ord/prog_params_data->rect_size.y), wrl->map_size.x)].is_selected = true; 

                            if (wrl->map[cell_id_in_map((ab/prog_params_data->rect_size.x), (ord/prog_params_data->rect_size.y), wrl->map_size.x)].land_type.game_id == LAND_BASIC)
                            {
                                ++world_params_data->cells_selected[0];
                            }
                            else if (wrl->map[cell_id_in_map((ab/prog_params_data->rect_size.x), (ord/prog_params_data->rect_size.y), wrl->map_size.x)].land_type.game_id == LAND_WATER)
                            {
                                world_params_data->cells_selected[1]++;
                            }
                            else if (wrl->map[cell_id_in_map((ab/prog_params_data->rect_size.x), (ord/prog_params_data->rect_size.y), wrl->map_size.x)].land_type.game_id == LAND_MOUNTAINS)
                            {
                                world_params_data->cells_selected[2]++;
                            }
                            else if (wrl->map[cell_id_in_map((ab/prog_params_data->rect_size.x), (ord/prog_params_data->rect_size.y), wrl->map_size.x)].land_type.game_id == LAND_ROCK)
                            {
                                world_params_data->cells_selected[3]++;
                            }
                            else if (wrl->map[cell_id_in_map((ab/prog_params_data->rect_size.x), (ord/prog_params_data->rect_size.y), wrl->map_size.x)].land_type.game_id == LAND_DEEP_WATER)
                            {
                                world_params_data->cells_selected[4]++;
                            }
                        }
                    }
                }

                square_selecting_freeze = 30;
            }
        } 
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) // STUB
        {
            if ((square_selecting_start_cell_coords.x/prog_params_data->rect_size.x) < prog_params_data->window_size.x && (square_selecting_start_cell_coords.x/prog_params_data->rect_size.x) > 0
              &&(square_selecting_start_cell_coords.y/prog_params_data->rect_size.y) < prog_params_data->window_size.y && (square_selecting_start_cell_coords.y/prog_params_data->rect_size.y) > 0)
            {
                wrl->map[cell_id_in_map((mouse_position.x/prog_params_data->rect_size.x), (mouse_position.y/prog_params_data->rect_size.y), wrl->map_size.x)].is_selected = false;

                wrl->map[cell_id_in_map((mouse_position.x/prog_params_data->rect_size.x) +1, (mouse_position.y/prog_params_data->rect_size.y), wrl->map_size.x)].is_selected = false;
                wrl->map[cell_id_in_map((mouse_position.x/prog_params_data->rect_size.x) -1, (mouse_position.y/prog_params_data->rect_size.y), wrl->map_size.x)].is_selected = false;

                wrl->map[cell_id_in_map((mouse_position.x/prog_params_data->rect_size.x), (mouse_position.y/prog_params_data->rect_size.y) +1, wrl->map_size.x)].is_selected = false;
                wrl->map[cell_id_in_map((mouse_position.x/prog_params_data->rect_size.x), (mouse_position.y/prog_params_data->rect_size.y) -1, wrl->map_size.x)].is_selected = false;
            }
        } 
        else if (IsKeyDown(KEY_ESCAPE))
        {
            if_square_selecting_active = !if_square_selecting_active;
            deselect_all_world_map(wrl, world_params_data);
        }

        EndDrawing();
    }

    CloseWindow();

    raw_log_to_file(log_data, LOGS_BARRIERS);
    log_to_file(log_data, "APP CORRECTLY CLOSED\n");
    raw_log_to_file(log_data, LOGS_BARRIERS);

    delete_world(wrl, world_params_data, log_data);
    delete_ui_lord(ui_central);

    free(window_name);

    free(source_log_file_path);
    fclose(log_data->source_log_file);

    undefine_all_data_lords(prog_params_data, world_params_data, draw_data, log_data);
}