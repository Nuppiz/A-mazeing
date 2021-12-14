#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dos.h>
#include <math.h>

#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels */
#define SCREEN_HEIGHT       200       /* height in pixels */

#define TILE_WIDTH          8
#define TILE_HEIGHT         8

#define GAME_M_MAIN         0
#define GAME_M_OPT          1
#define GAME_M_HLP          2
#define GAME_M_STOR         3
#define GAME_M_KCON         4
#define GAME_INGAME         5
#define GAME_OVER           6
#define GAME_WIN            7
#define GAME_END            8

#define MAX_ACTORS          10
#define LEVEL_SIZE          level_width * level_height

#define ACTOR_EMPTY         0
#define ACTOR_PLAYER        1
#define ACTOR_GUARD         2
#define ACTOR_GRAVE         3   
#define ACTOR_EXPLO         4

#define MAPSIZE             1000

#define LAST_LEVEL          3

#define MAP_WALL           87
#define MAP_FLOOR          45
#define MAP_DOOR_C         124
#define MAP_DOOR_O         250
#define MAP_EXIT           101
#define MAP_KEY            42
#define MAP_MINE           94

#define TICKS               (*(volatile unsigned long far *)(0x0040006CL))

//macros
#define TILE_AT(x, y)           g->tile_data[(y) * g->level_width + (x)]
#define SET_TILE(x, y, tile)    TILE_AT(x, y) = tile

struct Actor
{
    int8_t x, y, x_vel, y_vel, type, old_x, old_y;
    int8_t direction;
};

struct GameData
{   
    int game_running;
    int game_state;
    uint8_t debugmode;
    uint8_t music_enabled;
    /*int new_game;*/

    int level_num;
    int level_width;
    int level_height;
    int render_offset_x;
    int render_offset_y;
    uint8_t tile_data[MAPSIZE];
    uint8_t object_map[MAPSIZE];
    int8_t player_lives;
    int keys_acquired;
    int actor_count;

    struct Actor Actors[MAX_ACTORS];
};

struct Cursor
{
    int old_x;
    int old_y;
    int new_x;
    int new_y;
    int8_t selection;
};

enum SPRITES
{
    TILE_FLOOR,
    TILE_WALL,
    TILE_DOOR_C,
    TILE_DOOR_O,
    TILE_EXIT,
    ITEM_KEY,
    ITEM_MINE,
    SPR_PLAYER,
    SPR_GUARD,
    SPR_EXPLO,
    SPR_GRAVE,
    SPR_ERROR,
    SHAD_IN_COR,
    SHAD_HORZ,
    SHAD_VERT,
    SHAD_OUT_COR,
    SHAD_MINE,
    SHAD_KEY,
    NUM_SPRITES
};