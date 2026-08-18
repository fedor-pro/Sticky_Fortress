#include <generate.h>
#include <time.h>
#include <stdlib.h>

bool structures_overlay_check(world *wrl, landscape_structure structure, coord zone_start_coord, int zone_max_long_x, int zone_max_long_y, int l, int g) {
    if (wrl->map[cell_id_in_map((zone_start_coord.x + l), (zone_start_coord.y + g), wrl->map_size.x)].land_type.game_id != wrl->world_landscapes[0].game_id) { // Overlay check

        free(structure.incoming_cells_coords);
        return true;
    }

    return false;
}

void generate_structure(world *wrl)
{
    landscape_structure structure;

    float pond = rand() % 100 + rand() % 15;
    float pond_increment_step = 1 + rand () % 3;
    int pond_border = 200 + rand() % 100;

    if (rand() % 10 > 5)
    {
        structure.landscape = wrl->world_landscapes[1];
        structure.second_landscape = wrl->world_landscapes[2];
    }
    else
    {
        structure.landscape = wrl->world_landscapes[3];
        structure.second_landscape = wrl->world_landscapes[4];
    }

    coord zone_start_coord = (coord){rand() % wrl->map_size.x, rand() % wrl->map_size.y};

    int zone_max_long_x = 3 + rand() % 20;
    int zone_max_long_y = 3 + rand() % 10;

    printf("check_1\n");

    structure.incoming_cells_coords = malloc(sizeof(landscape_cell) * zone_max_long_y * zone_max_long_y);

    printf("check_2\n");

    int j = 0;

    while (!((zone_start_coord.x + zone_max_long_x) < wrl->map_size.x && (zone_start_coord.y + zone_max_long_y) < wrl->map_size.y))
    {
        zone_start_coord.x = 2 + rand() % 5;
        zone_start_coord.y = 2 + rand() % 5;
    }

    for (int l = 0; l < zone_max_long_x; l++)
    {
        for (int g = 0; g < zone_max_long_y; g++)
        {
            if (!(structure.landscape.game_id == LAND_MOUNTAINS)) // Water
            {
                structure.incoming_cells_coords[j].x = zone_start_coord.x + l;
                structure.incoming_cells_coords[j].y = zone_start_coord.y + g;

                if (structures_overlay_check(wrl, structure, zone_start_coord, zone_max_long_x, zone_max_long_y, l, g)) {
                    return;
                }

                if ((zone_max_long_y/2) + (rand() % pond_border)/2 >= pond) {
                    wrl->map[cell_id_in_map((zone_start_coord.x + l), (zone_start_coord.y + g), wrl->map_size.x)].land_type = structure.second_landscape;
                } else {
                    wrl->map[cell_id_in_map((zone_start_coord.x + l), (zone_start_coord.y + g), wrl->map_size.x)].land_type = structure.landscape;
                } // So ugly generation
                
                j++;
            }
            else // Mountains
            {
                if (((rand() % pond_border) - 20) > pond)
                {
                    structure.incoming_cells_coords[j].x = zone_start_coord.x + l;
                    structure.incoming_cells_coords[j].y = zone_start_coord.y + g;

                    if (structures_overlay_check(wrl, structure, zone_start_coord, zone_max_long_x, zone_max_long_y, l, g)) {
                        return;
                    }

                    if (((rand() % pond_border + pond/2))/2 >= pond) {
                        wrl->map[cell_id_in_map((zone_start_coord.x + l), (zone_start_coord.y + g), wrl->map_size.x)].land_type = structure.second_landscape;
                    } else {
                        wrl->map[cell_id_in_map((zone_start_coord.x + l), (zone_start_coord.y + g), wrl->map_size.x)].land_type = structure.landscape;
                    }

                    j++;
                }
            }

            pond += pond_increment_step;
        }
    }
}

void generate_world_structures(world *wrl, world_params_data_lord *world_params_data)
{
    for (int x = 0; x < world_params_data->structures_number; x++)
    {
        printf("Generating %d\n", x);
        generate_structure(wrl);
    }

    return;
}