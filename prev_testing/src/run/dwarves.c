#include "dwarves.h"
#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include "logging.h"

// Aaaaaaaargh
// TODO: Fix all


void move_dwarf(direction direct, dwarf *dw, world *wrl)
{
    switch (direct)
    {
    case RIGHT:
        if (dw->coords.x + 1 < wrl->map_size.x && wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].land_type.can_be_occupied)
        {
            wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].is_occupied = false;
            dw->coords.x++;

            wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].is_occupied = true;
        }

        break;
    case LEFT:
        if (dw->coords.x - 1 >= 0 && wrl->map[cell_id_in_map(dw->coords.x - 1, dw->coords.y, wrl->map_size.x)].land_type.can_be_occupied)
        {
            wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].is_occupied = false;
            dw->coords.x--;

            wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].is_occupied = true;
        }

        break;
    case DOWN:
        if (dw->coords.y + 1 < wrl->map_size.y && wrl->map[cell_id_in_map(dw->coords.x, dw->coords.y + 1, wrl->map_size.x)].land_type.can_be_occupied)
        {
            wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].is_occupied = false;
            dw->coords.y++;

            wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].is_occupied = true;
        }

        break;
    case UP:
        if (dw->coords.y - 1 >= 0 && wrl->map[cell_id_in_map(dw->coords.x, dw->coords.y - 1, wrl->map_size.x)].land_type.can_be_occupied)
        {
            wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].is_occupied = false;
            dw->coords.y--;

            wrl->map[cell_id_in_map(dw->coords.x + 1, dw->coords.y, wrl->map_size.x)].is_occupied = true;
        }

        break;
    }
}

void restore_hunger_dwarf(world  *wrl, dwarf *dw, world_params_data_lord *world_params_data, log_data_lord *log_data)
{
    if (wrl->items[dw->target_food_id].number > 0)
    {        
        wrl->items[dw->target_food_id].number--;

        dw->hunger -= 10 + rand() % 10;
        if (dw->hunger < 0) {
            dw->hunger = 0;
        }
    }   
}

bool find_nearest_food(world *wrl, dwarf *dw, world_params_data_lord *world_params_data)
{
    int min_distance = 1000;
    bool is_exist_any_food = false;

    for (int z = 0; z < world_params_data->start_food_on_map; z++) // Finding nearest food
    {
        if (wrl->items[z].number > 0 && (abs(dw->coords.x - wrl->items[z].coords.x) + abs(dw->coords.y - wrl->items[z].coords.y)) < min_distance)
        {
            dw->target_food_id = z;
            min_distance = abs(dw->coords.x - wrl->items[z].coords.x) + abs(dw->coords.y - wrl->items[z].coords.y);

            dw->target_cell_coords.x = wrl->items[z].coords.x;
            dw->target_cell_coords.y = wrl->items[z].coords.y;

            is_exist_any_food = true;
        }
    }

    if (is_exist_any_food)
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;
}

void update_dwarf(world *wrl, coord map_size, dwarf *dw, int timer, world_params_data_lord *world_params_data, log_data_lord *log_data) // Updating dwarf
{
    int random_for_move = rand() % 5;

    bool is_reached_target_food = (dw->coords.x == wrl->items[dw->target_food_id].coords.x && dw->coords.y == wrl->items[dw->target_food_id].coords.y && wrl->items[dw->target_food_id].number > 0);
    bool is_enough_hunger_reached = dw->hunger >= (dw->die_level_hunger * 0.60);

    if (dw->dwarvenness == true)
    {
        if (!is_enough_hunger_reached)
        {
            dw->moving_state = UNTARGET_MOVING; // Boring
        }

        if (is_enough_hunger_reached && !is_reached_target_food)
        {
            dw->moving_state = TARGETING;

            if (!find_nearest_food(wrl, dw, world_params_data)) // Find nearest food, if wasn't finded - boring
            {
                dw->moving_state = UNTARGET_MOVING;
                dw->target_food_id = -1;
            }
        }
        else if (is_enough_hunger_reached && is_reached_target_food) // Eating
        {
            dw->moving_state = EATING;
        }

        if (timer % 10 == 0) // Moving and eating
        {
            if (dw->moving_state == UNTARGET_MOVING) // Random moving
            {
                switch (random_for_move) {
                    case 1:
                        move_dwarf(RIGHT, dw, wrl);
                    break;
                    case 2:
                        move_dwarf(LEFT, dw, wrl);
                    break;
                    case 3:
                        move_dwarf(DOWN, dw, wrl);
                    break;
                    case 4:
                        move_dwarf(UP, dw, wrl);
                    break;
                }

            }
            else if (dw->moving_state == TARGETING) // Go to target
            {
                if (dw->coords.x < dw->target_cell_coords.x)
                {
                    move_dwarf(RIGHT, dw, wrl);
                }
                else if (dw->coords.x > dw->target_cell_coords.x)
                {
                    move_dwarf(LEFT, dw, wrl);
                }

                else if (dw->coords.y < dw->target_cell_coords.y)
                {
                    move_dwarf(DOWN, dw, wrl);
                }
                else if (dw->coords.y > dw->target_cell_coords.y)
                {
                    move_dwarf(UP, dw, wrl);
                }
            }
            else if (dw->moving_state == EATING) // Eating
            {
                if (wrl->items[dw->target_food_id].number <= 0)
                {
                    dw->moving_state = TARGETING;

                    if (!find_nearest_food(wrl, dw, world_params_data))
                    {
                        dw->moving_state = UNTARGET_MOVING;
                        dw->target_food_id = -1;
                    }
                }
                else
                {
                    restore_hunger_dwarf(wrl, dw, world_params_data, log_data);
                }
            }
        }

        if (timer % 1 == 0)
        {
            dw->hunger += 0.001;
            if (rand() % 2 > 0) {
                dw->hunger += 0.0005;
            }

            if (dw->hunger >= dw->die_level_hunger) // Die from hunger
            {
                dw->is_alive = false;
                log_to_file(log_data, "Dwarf with id |");
                raw_log_to_file(log_data, dw->game_id);
                raw_log_to_file(log_data, "| died from hunger\n");
            }
        }

        dw->drawing_color = GREEN;
        world_params_data->dwarves_alive ++;

        if (wrl->map[cell_id_in_map(dw->coords.x, dw->coords.y, wrl->map_size.x)].is_selected)
        {
            dw->drawing_color = RED;
            world_params_data->dwarves_selected ++;
        }
        else if (!dw->is_alive)
        {
            dw->drawing_color = GRAY;
            world_params_data->dwarves_alive --;
        }
        else if (dw->moving_state == EATING)
        {
            dw->drawing_color = ORANGE;
        } else if (is_enough_hunger_reached)
        {
            if (timer > 20) {
                dw->drawing_color = BLUE;
            } else {
                dw->drawing_color = GREEN;
            }
        }
    }
}