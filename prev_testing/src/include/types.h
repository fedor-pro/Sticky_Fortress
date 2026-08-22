#if !defined(TYPES_H)
#define TYPES_H

// ✗✗✗ RESTRICTED ✗✗✗ #define LL() printf("Check at %s : %d passed succesfully\n", __FILE__, __LINE__) ✗✗✗ RESTRICTED ✗✗✗

#define cell_id_in_map(x, y, map_size_x) ((x) + ((map_size_x) * (y)))

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>

typedef struct
{
    int x;
    int y;
} coord;

typedef enum
{
    LAND_BASIC,
    LAND_WATER,
    LAND_DEEP_WATER,
    LAND_MOUNTAINS,
    LAND_ROCK
} landscape_type_id;

typedef enum
{
    RIGHT,
    LEFT,
    UP,
    DOWN
} direction;

typedef enum
{
    UNTARGET_MOVING,
    TARGETING,
    EATING
} dwarf_moving_state;

typedef struct
{
    coord start_coords;

    coord canv_size_coords;

    Color background_color;
} gui_pannel;

typedef struct
{
    char *text;

    coord start_coords;

    int font_size;

    Color font_color;
} gui_text;

typedef struct
{
    landscape_type_id game_id;
    char *game_name;
    bool can_be_occupied;

    Color draw_color;
} landscape_type;

typedef struct
{
    coord l_coord;

    bool is_selected;
    bool is_occupied;

    landscape_type land_type;
} landscape_cell;

typedef struct
{
    char *game_id;
    char *game_name;

    bool dwarvenness;
    bool is_alive;
    char *char_value;

    coord coords;

    dwarf_moving_state moving_state;
    int target_food_id;

    coord target_cell_coords;

    float hunger;
    float die_level_hunger;
    float sleepiness;

    Color drawing_color;
} dwarf;

typedef enum
{
    FOOD
} item_type;

typedef struct
{
    coord coords;

    item_type im_type;
    char *char_value;

    float number;
    bool is_exist;
} item;

typedef struct
{
    coord *incoming_cells_coords;

    landscape_type landscape;
    landscape_type second_landscape;
} landscape_structure;

typedef struct
{
    char *world_name;
    FILE *world_file;

    landscape_cell *map;
    coord map_size;

    item *items;
    dwarf *dwarves;

    landscape_type *world_landscapes;
} world;

typedef struct
{
    gui_pannel *all_gui_pannels;
    gui_text *all_gui_text;
} ui_lord;

typedef struct
{
    char *default_name;

    int start_dwarves_number;
    int start_food_on_map;
    int structures_number;

    int dwarves_alive;
    int dwarves_selected;
    int food_exists;

    int *cells_selected;
} world_params_data_lord;

typedef struct
{
    coord window_size;
    coord rect_size;
    int text_buffer_size;

    int timer;
    int current_fps;
    bool is_paused;

    int timer_reset;
    char *string_current_fps;

    bool if_square_selecting_active;
    coord square_selecting_start_cell_coords;
    coord square_selecting_end_cell_coords;
    int square_selecting_freeze;

    coord mouse_position;
} prog_params_data_lord;

typedef struct
{
    int default_font_size;

    char *default_human_char;
    char *default_food_char;
} draw_data_lord;

typedef struct
{
    struct tm *tm; 
    FILE *source_log_file;
    time_t raw_time;
} log_data_lord;

#endif